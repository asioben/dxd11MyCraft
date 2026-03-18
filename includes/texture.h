#ifndef TEXTURE_H
#define TEXTURE_H

//include headers
#include "d3d.h"

struct SPRITE{
    float u;
    float v;
    float width;
    float height;
};

struct TEXTURE{
    //date
    ID3D11Texture2D *texture = NULL;
    ID3D11SamplerState *samplerState = NULL;
    ID3D11ShaderResourceView *textureView = NULL;

    //functions
    private: bool loadTexture(const char* filename, ID3D11Device *device);
    public:
    SPRITE getSprite(int index, int rows, int columns);
    bool createTexture(const char* filename, ID3D11Device *device);
    void destroyTexture();  
};

#endif