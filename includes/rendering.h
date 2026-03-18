#ifndef RENDERING_H
#define RENDERING_H

//include headers
#include "camera.h"
#include "texture.h"
#include "chunk.h"

//colour
struct float4{
    float r;
    float g;
    float b;
    float a;
};

//instance
struct INSTANCE{
    XMMATRIX World;
};

struct VOXEL{
    //data
    public:
    ID3D11Buffer *vertices = NULL;
    ID3D11Buffer *indices = NULL;
    ID3D11Buffer *instancesBuffer = NULL;
    ID3D11Buffer *texturesBuffer = NULL;
    ID3D11ShaderResourceView *structuredSRV = NULL;

    //trigger
    private:
    bool initInstances = false;

    //functions
    public:
    void drawCube(D3D *d3d, CHUNKMANAGER *chunks, TEXTURE *texture, CAMERA *camera);
    void destroyCube();
    private:
    bool initDrawing(ID3D11Device *device, ID3D11DeviceContext *deviceContext, TEXTURE *texture, CHUNKMANAGER *chunks);
};

bool initVertexBuffer(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ID3D11Buffer *&vertices, ID3D11Buffer *&indices, const void *vertex, int *indice, size_t size, int indices_size);

/*struct RENDERING{
    void update();
    void rendering();
};*/

#endif