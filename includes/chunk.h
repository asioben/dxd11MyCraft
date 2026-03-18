#ifndef CHUNK_H
#define CHUNK_H

//headers
#include "block.h"
#include "input.h"
#include "camera.h"
#include "perlin.h"

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <vector>
#include <stdio.h>

using namespace std;
using namespace DirectX;

struct BLOCK{
    XMMATRIX World;
    float3 position;
    float3 boxMin;
    float3 boxMax;
    BLOCKID id;
};

struct CHUNK{
    public:
    vector<VERTEX> faces;
    bool chunkUpdated = false;

    BoundingBox box;
    float2 position;

    private:
    unsigned int size = 0;
    float3 chunkSize = {16.0f,16.0f,16.0f};
    BLOCKID blocks[16][16][16];
    vector <BLOCK> cubes;
    vector<XMMATRIX> WorldsToRender;

    public:
    void initChunk(int seed, TEXTURE *texture);
    void updateChunk(INPUT_ *input, CAMERA *camera, UINT width, UINT height, TEXTURE *texture);

    //optimization algorithm
    private:
    void chunkBoundingBox();
    void chunkVisibleBlocks(TEXTURE *texture);
};

struct CHUNKMANAGER{
    vector<CHUNK> chunks;
    vector<CHUNK> toRender;
    vector<VERTEX> faces;
    vector<int> indices;
    int currentChunk = 0;

    void initChunks(TEXTURE *texture);
    void frustumCulling(CAMERA *camera);
    void distanceCulling(CAMERA *camera);
    int toRenderChunks();
};

bool rayIntersectCube(const XMFLOAT3 &rayO, const XMFLOAT3 &rayDir, const XMFLOAT3 &boxMin, const XMFLOAT3 &boxMax, float &tminOut);

#endif