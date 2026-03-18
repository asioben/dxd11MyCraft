#ifndef SKYBOX_H
#define SKYBOX_H

//headers
#include "rendering.h"

struct SPECIAL_VERTEX{
    float3 position;
    float4 color;
};

struct SKYBOX{
    public:
    ID3D11Buffer *vertices = NULL;
    ID3D11Buffer *indexes = NULL;
    ID3D11DepthStencilState *depthStencilState = NULL;
    ID3D11RasterizerState *rasterizerState = NULL;
    ID3D11VertexShader *vertexShader = NULL;
    ID3D11PixelShader *pixelShader = NULL;
    ID3D11InputLayout *inputLayout = NULL;
    ID3D11Texture2D *depthStencilBuffer = NULL;

    int width;
    int height;

    public:
    bool init(ID3D11Device *device, ID3D11DeviceContext *deviceContext, const WCHAR *vFile, const WCHAR *pFile, const char *vFunction, const char *pFunction);
    private: bool initDrawing(ID3D11DeviceContext *deviceContext);
    public:
    void draw(ID3D11Device *device, ID3D11DeviceContext *deviceContext, CAMERA *camera);
    void destroy();
};

#endif