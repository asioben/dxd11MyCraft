#ifndef D3D_H
#define D3D_H

//include DirectX header
#include <d3d11.h>
#include <d3dcompiler.h>

//include headers
#include <windows.h>
#include <iostream>
//#include <wrl/client.h>

#define SAFE_RELEASE(x) if(x)x->Release()

struct D3D{
    public:
    //data
    ID3D11Device *device = NULL;
    ID3D11DeviceContext *deviceContext = NULL;
    IDXGISwapChain *swapChain = NULL;
    ID3D11RenderTargetView *renderTargetView = NULL;
    ID3D11VertexShader *vertexShader = NULL;
    ID3D11PixelShader *pixelShader = NULL;
    ID3D11InputLayout *inputLayout = NULL;
    ID3D11DepthStencilView *depthStencilView = NULL;
    ID3D11DepthStencilState *depthStencilState = NULL;
    ID3D11Texture2D *depthStencilBuffer = NULL;
    ID3D11RasterizerState *rasterizerState = NULL;

    HWND hWnd;
    UINT width;
    UINT height;
    
    //functions
    public:
    bool init();
    bool renderFrame();
    void destroy();
};

bool initShaders(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ID3D11VertexShader *&vertexShader, ID3D11PixelShader *&pixelShader, ID3D11InputLayout *&inputLayout, const WCHAR *vFile, const WCHAR *pFile, const char *vFunction, const char *pFunction, D3D11_INPUT_ELEMENT_DESC *ied, UINT iedCount);

bool error(const char *message);

#endif