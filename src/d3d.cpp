#include "../includes/d3d.h"
#include <stdio.h>

//some function prototype
bool getShaderError(ID3D10Blob *errorBlob, const char *message, HRESULT hr);

bool D3D::init(){
    //init swap chain struct
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd,sizeof(scd));

    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHARED;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = true;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    //support for d2d and d3d interopedability
    UINT createDeviceFlags =  D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    D3D_FEATURE_LEVEL featureLevels[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0
    };

    D3D_FEATURE_LEVEL featureLevel;

    //create a device, device context and swap chain with the scd 
    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,createDeviceFlags,featureLevels,ARRAYSIZE(featureLevels),D3D11_SDK_VERSION,&scd,&swapChain,&device,&featureLevel,&deviceContext);
    if(FAILED(hr)) return error("create swap chain failed\n");

    //get the backbuffer adress
    ID3D11Texture2D *backBuffer;
    hr = swapChain->GetBuffer(0,__uuidof(backBuffer),(LPVOID*)&backBuffer);
    if(FAILED(hr)) return error("get the buffer failed\n");

    //create the render target view with the back buffer adress
    hr = device->CreateRenderTargetView(backBuffer,NULL,&renderTargetView);
    backBuffer->Release();
    if(FAILED(hr)) return error("create the render target view failed\n");

    //set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport,sizeof(viewport));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<FLOAT>(width);
    viewport.Height = static_cast<FLOAT>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    deviceContext->RSSetViewports(1,&viewport);

    //the desc struct for depth stencil view
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc,sizeof(D3D11_TEXTURE2D_DESC));

    //fill the depth stencil desc struct
    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count =  1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    if(FAILED(device->CreateTexture2D(&depthStencilDesc,NULL,&depthStencilBuffer)))return error("Depth Stencil View texture/buffer creation failed\n");

    //new struct
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory(&dsDesc,sizeof(D3D11_DEPTH_STENCIL_DESC));

    //depth test parameters
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    //create depth stencil state
    if(FAILED(device->CreateDepthStencilState(&dsDesc,&depthStencilState))) return error("Depth Stencil State creation failed\n");

    //new struct
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc,sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

    //fill this new struct desc
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    dsvDesc.Flags = 0;

    //create the depth stencil view
    if(FAILED(device->CreateDepthStencilView(depthStencilBuffer,&dsvDesc,&depthStencilView)))return error("Depth Stencil View Creation failed\n");

    //struct desc about rasterizer state
    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory(&rasterDesc,sizeof(D3D11_RASTERIZER_DESC));

    //fill this struct desc
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = false;
    
    //create the rasterizer state
    if(FAILED(device->CreateRasterizerState(&rasterDesc,&rasterizerState))) return error("Rasterizer State Creation failed\n");

    //set it
    deviceContext->RSSetState(rasterizerState);

    //set the render target as the backbuffer
    deviceContext->OMSetRenderTargets(1,&renderTargetView,depthStencilView);

    return true;
}

bool D3D::renderFrame(){
    float color[4] = {0.0f,0.0f,0.0f,0.0f};

    //clear the back buffer
    deviceContext->ClearRenderTargetView(renderTargetView,color);

    //sitch the backbuffer and the frontbuffer
    HRESULT hr = swapChain->Present(0,0);
    if(FAILED(hr)) return error("present failed\n");

    return true;
}

bool getShaderError(ID3D10Blob *errorBlob, const char *message, HRESULT hr){
    printf("%08x\n",hr);
    if(errorBlob)std::cerr << "Error: " << (char*)errorBlob->GetBufferPointer();
    return error("Vertex Shader compiling failed\n");
}

bool initShaders(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ID3D11VertexShader *&vertexShader, ID3D11PixelShader *&pixelShader, ID3D11InputLayout *&inputLayout, const WCHAR *vFile, const WCHAR *pFile, const char *vFunction, const char *pFunction, D3D11_INPUT_ELEMENT_DESC *ied, UINT iedCount){
    //load and compile shaders
    ID3D10Blob *vs,*ps,*errorBlob = NULL;
    HRESULT hr = D3DCompileFromFile(vFile,NULL,NULL,vFunction,"vs_4_0",D3D10_SHADER_ENABLE_STRICTNESS,0,&vs,&errorBlob);
    if(FAILED(hr)) return getShaderError(errorBlob,"Vertex Shader compiling failed \n",hr);
    if(FAILED(D3DCompileFromFile(pFile,NULL,NULL,pFunction,"ps_4_0",D3D10_SHADER_ENABLE_STRICTNESS,0,&ps,NULL))) return getShaderError(errorBlob,"Pixel Shader compiling failed \n",hr);
    //encapsulate shaders into shaders object
    hr = device->CreateVertexShader(vs->GetBufferPointer(),vs->GetBufferSize(),NULL,&vertexShader);
    if(FAILED(hr)) return getShaderError(errorBlob,"Create Vertex Shader failed \n",hr);
    if(FAILED(device->CreatePixelShader(ps->GetBufferPointer(),ps->GetBufferSize(),NULL,&pixelShader))) return getShaderError(errorBlob,"Create Pixel Shader failed \n",hr);
    //set the shader object
    deviceContext->VSSetShader(vertexShader,NULL,0);
    deviceContext->PSSetShader(pixelShader,NULL,0);
    //the vertex struct that draw the triangle, you specify to the gpu which part is what
    hr = device->CreateInputLayout(ied,iedCount,vs->GetBufferPointer(),vs->GetBufferSize(),&inputLayout);
    if(FAILED(hr))return getShaderError(errorBlob,"Create input layout failed \n",hr);
    deviceContext->IASetInputLayout(inputLayout);
    return true;
}

void D3D::destroy(){
    SAFE_RELEASE(device);
    SAFE_RELEASE(deviceContext);
    SAFE_RELEASE(swapChain);
    SAFE_RELEASE(vertexShader);
    SAFE_RELEASE(pixelShader);
    SAFE_RELEASE(inputLayout);
    SAFE_RELEASE(renderTargetView);
    SAFE_RELEASE(depthStencilView);
    SAFE_RELEASE(depthStencilState);
    SAFE_RELEASE(depthStencilBuffer);
    SAFE_RELEASE(rasterizerState);
}

bool error(const char *message){
    std::cout << message;
    return false;
}