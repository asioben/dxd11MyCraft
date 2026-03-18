#ifndef BLOCK_H
#define BLOCK_H

#include "texture.h"

enum BLOCKID{
    GRASS,
    DIRT,
    AIR,
    STONE
};

//tex coord
struct float2{
    float u;
    float v;
};

//position
struct float3{
    float x;
    float y;
    float z;
};

//vertex
struct VERTEX{
    float3 position;
    float2 texcoord;
};

void getBlock(BLOCKID block, int *indexes);

void getFace(BLOCKID block, TEXTURE *texture, int face, VERTEX *vertex);

#endif