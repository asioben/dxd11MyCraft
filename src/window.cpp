#include "../includes/window.h"

//global
bool FULLSCREEN_ = true;
RAWKEYBOARD kb;
RAWMOUSE rm;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
        case WM_INPUT:{
            //get raw input data
            UINT dwSize = 0;
            GetRawInputData((HRAWINPUT)lParam,RID_INPUT,NULL,&dwSize,sizeof(RAWINPUTHEADER));
            BYTE *lpb = new BYTE[dwSize];
            if(lpb == NULL) return 0;
            if(GetRawInputData((HRAWINPUT)lParam, RID_INPUT,lpb,&dwSize,sizeof(RAWINPUTHEADER))!= dwSize) return error("Error getting raw input\n");

            RAWINPUT *raw = (RAWINPUT*) lpb;
            //get keyboard input
            if(raw->header.dwType == RIM_TYPEKEYBOARD)kb = raw->data.keyboard;
            //get mouse input
            if(raw->header.dwType == RIM_TYPEMOUSE)rm = raw->data.mouse;

            delete[] lpb;
            return 0;
        }
        //check if the window is destroyed
        case WM_DESTROY:{
            PostQuitMessage(0);
            return 0;
        }
        //check if it's close
        case WM_CLOSE:{
            PostQuitMessage(0);
            return 0;
        }default: return DefWindowProc(hWnd,msg,wParam,lParam);
    }
}

bool WINDOW::init(){
    //struct for windows class
    WNDCLASSEX wc;

    //clear out the class
    ZeroMemory(&wc,sizeof(wc));

    if(fullscreen){if(!fullScreen(wc))return error("FullScreen failed\n");}
    else{if(!initWindow(wc)){return error("Init simple window failed\n");}}

    //register for raw input
    RAWINPUTDEVICE rid[2];

    //keyboard
    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = 0x06;
    rid[0].dwFlags = RIDEV_INPUTSINK;
    rid[0].hwndTarget = hWnd;

    //mouse
    rid[1].usUsagePage = 0x01;
    rid[1].usUsage = 0x02;
    rid[1].dwFlags = RIDEV_INPUTSINK;
    rid[1].hwndTarget = hWnd;

    if(!RegisterRawInputDevices(rid,2,sizeof(rid[0]))){
        DWORD err = GetLastError();
        std::cout << "error: " << err;
        return error("Register raw input failed\n");
    }

    return true;
}

void WINDOW::destroy(){
    //show if the cursor is hidden
    ShowCursor(true);

    //if fullscreen mode, fix the display settings
    if(fullscreen) ChangeDisplaySettings(NULL,0);

    //destroy the window
    DestroyWindow(hWnd);
    hWnd = NULL;

    //destroy app instance
    UnregisterClass(name,hInstance);
    hInstance = NULL;
}

void WINDOW::update(){
    MSG msg;
    bool loop = true;

    //init msg
    ZeroMemory(&msg,sizeof(msg));

    int fps = 0;
    int fps_ = 0;
    float3 camPos;

    chunk_->initChunks(texture);

    //loop
    while(loop){
        //handle windows msg
        if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
            //translate the keystroke un the right format
            TranslateMessage(&msg);
            //send the message to WndProc
            DispatchMessage(&msg);
        }
        //if windows signals end , quit
        if(msg.message == WM_QUIT) loop = false;
        input->keyboard = kb;
        input->mouse = rm;
        if(input->keyboard_input() == VK_ESCAPE) FULLSCREEN_ = false;
        if(FULLSCREEN_ == false && fullscreen == true) loop = false;
        float4 color = {130.0f,171.0f,254.0f,0.0f};
        float colors[4] = {color.r/255.0f,color.g/255.0f,color.b/255.0f,color.a/255.0f};
        /*if(width >= 6282) width = 0;
        width += 1;
        camera->World = XMMatrixRotationY((float)width/1000);*/
        //chunk_->frustumCulling(camera);
        camera->movement(input,time[1].deltaTime());
        time[1].start = time[1].end;
        chunk_->toRender[chunk_->currentChunk].updateChunk(input,camera,width,height,texture);
        //chunk_->distanceCulling(camera);
        d3d->deviceContext->ClearRenderTargetView(d3d->renderTargetView,colors);
        d3d->deviceContext->ClearDepthStencilView(d3d->depthStencilView,D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL,1.0f,0);
        d3d->deviceContext->OMSetDepthStencilState(d3d->depthStencilState,0);
        //skybox->draw(d3d->device,d3d->deviceContext,camera);
        chunk_->toRenderChunks();
        camPos.x = XMVectorGetX(camera->position);
        camPos.y = XMVectorGetY(camera->position);
        camPos.z = XMVectorGetZ(camera->position);
        cube->drawCube(d3d,chunk_,texture,camera);
        D2D1_RECT_F rect = RectF(10,10,400,100);
        std::wostringstream text_;
        text_ << L"FPS: " << fps_;
        D2D1_RECT_F rect0 = RectF(10,30,400,100);
        text->renderText(text_.str().c_str(),&rect);
        std::wostringstream text_0;
        text_0 << L"X: " << camPos.x << L"; Y: " << camPos.y << L"; Z: " << camPos.z;
        text->renderText(text_0.str().c_str(),&rect0);
        d3d->swapChain->Present(0,0);
        fps += 1;
        if(time[0].counter(1.0)){
            fps_ = fps;
            fps = 0;
        }
        //d3d->renderFrame();
   }
}

bool WINDOW::initWindow(WNDCLASSEX &wc){
      //fill the struct
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = name;
    wc.lpszMenuName = NULL;
    wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    wc.hIconSm = wc.hIcon;

    //register the class
    RegisterClassEx(&wc);

    //adjust the window size
    RECT wr = {0,0,(LONG)width,(LONG)height};
    if(!AdjustWindowRect(&wr,WS_EX_OVERLAPPEDWINDOW,FALSE)) return error("error adjusting window\n");

    //create the window
    hWnd = CreateWindowEx(0,name,name,WS_OVERLAPPEDWINDOW,posX,posY,wr.right-wr.left,wr.bottom-wr.top,NULL,NULL,hInstance,NULL);
    if(!hWnd) return error("Create Window failed\n");

    //display the window
    ShowWindow(hWnd,SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    
    //if the cursor is hidden
    ShowCursor(true);
    
    return true;
}

bool WINDOW::fullScreen(WNDCLASSEX &wc){
    //set the position at (0,0)
    posX = posY = 0;
    DEVMODE dm;

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL,IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = name;
    wc.cbSize = sizeof(WNDCLASSEX);

    //register the window class
    RegisterClassEx(&wc);

    //screen resolution
    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);

    //set to the max size of the desktop
    memset(&dm,0,sizeof(dm));
    dm.dmSize = sizeof(dm);
    dm.dmPelsWidth = (long)width;
    dm.dmPelsHeight = (long)height;
    dm.dmBitsPerPel = 32;
    dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    //change the dispay settings to full screen
    ChangeDisplaySettings(&dm,CDS_FULLSCREEN);

    //create fullscreen window
    hWnd = CreateWindowEx(WS_EX_APPWINDOW,name,name,WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,posX,posY,width,height,NULL,NULL,hInstance,NULL);

    if(!hWnd) return error("Create Window failed\n");

    //display the windows and set it as the focus
    ShowWindow(hWnd,SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);

    //hide the mouse cursor
    ShowCursor(false);

    return true;
}