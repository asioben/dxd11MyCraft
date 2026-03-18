//include headers
#include "../includes/window.h"
#include "../includes/d3d.h"
#include "../includes/input.h"
#include "../includes/texture.h"
#include "../includes/rendering.h"
#include "../includes/block.h"
#include "../includes/d2dWrite.h"

//
#define WIDTH 800
#define HEIGHT 600

//functions
inline int failed(WINDOW *window, D3D *d3d, TEXTURE *texture, VOXEL *cube, CAMERA *camera, TEXT *text);

inline int failed(WINDOW *window, D3D *d3d, TEXTURE *texture, VOXEL *cube, CAMERA *camera, TEXT *text){
    window->destroy();
    d3d->destroy();
    if(texture != NULL) texture->destroyTexture();
    if(cube != NULL) cube->destroyCube();
    if(camera != NULL) camera->destroy();
    if(text != NULL) text->destroy();
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
    //variables
    WINDOW window;
    D3D d3d;
    INPUT_ input;
    TEXTURE texture;
    VOXEL cube;
    CAMERA camera;
    TEXT text;
    TIME time[2];
    CHUNKMANAGER chunk_;

    int indexes[6];
    SPRITE sprites[6];

    const WCHAR* shadersFile = L"C:\\Users\\beni\\OneDrive\\Documents\\myCraft\\shaders\\shader.hlsl";
    const WCHAR* skyboxFile = L"C:\\Users\\beni\\OneDrive\\Documents\\myCraft\\shaders\\skybox.hlsl";
    D3D11_INPUT_ELEMENT_DESC ied[2] = {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},

        /*{"WORLD",0,DXGI_FORMAT_R32G32B32A32_FLOAT,1,0,D3D11_INPUT_PER_INSTANCE_DATA,1},
        {"WORLD",1,DXGI_FORMAT_R32G32B32A32_FLOAT,1,16,D3D11_INPUT_PER_INSTANCE_DATA,1},
        {"WORLD",2,DXGI_FORMAT_R32G32B32A32_FLOAT,1,32,D3D11_INPUT_PER_INSTANCE_DATA,1},
        {"WORLD",3,DXGI_FORMAT_R32G32B32A32_FLOAT,1,48,D3D11_INPUT_PER_INSTANCE_DATA,1},*/

    };

    //initialization window
    window.name = L"MyCraft";
    window.hInstance = hInstance;
    window.width = WIDTH;
    window.height = HEIGHT;
    window.posX = 300;
    window.posY = 200;
    window.d3d = &d3d;
    window.fullscreen = true;
    window.input = &input;
    window.texture = &texture;
    window.cube =  &cube;
    window.camera = &camera;
    window.text = &text;
    window.time = time;
    window.chunk_ = &chunk_;

    if(!window.init()){
        window.destroy();
        return 0;
    }

    //d3d
    d3d.hWnd = window.hWnd;
    d3d.width = window.width;
    d3d.height = window.height;
    if(!d3d.init())return failed(&window,&d3d,NULL,NULL,NULL,NULL);
    if(!initShaders(d3d.device,d3d.deviceContext,d3d.vertexShader,d3d.pixelShader,d3d.inputLayout,shadersFile,shadersFile,"VShader","PShader",ied,2))return failed(&window,&d3d,NULL,NULL,NULL,NULL);

    //texture
    if(!texture.createTexture("C:\\Users\\beni\\OneDrive\\Documents\\myCraft\\textures\\texture.png",d3d.device)) return failed(&window,&d3d,&texture,NULL,NULL,NULL);

    //camera init
    if(!camera.init(d3d.device,(float)WIDTH,(float)HEIGHT)) return failed(&window,&d3d,&texture,&cube,&camera,NULL);

    //text init
    if(!text.init(d3d.swapChain)) return failed(&window,&d3d,&texture,&cube,&camera,&text);

    //time init
    time[0].initTime();
    time[1].initTime();

    //update
    window.update();

    //releasing/cleaning
    failed(&window,&d3d,&texture,&cube,&camera,&text);
    return 0;
}
