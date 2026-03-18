#ifndef CAMERA_H
#define CAMERA_H

//headers
#include <DirectXMath.h>
#include "d3d.h"
#include "input.h"

using namespace DirectX;

struct CONSTANT_BUFFER_OBJECT{
    XMMATRIX VP;
};

struct CAMERA{
    //buffer
    ID3D11Buffer *constantBuffer;
    CONSTANT_BUFFER_OBJECT cb;

    //viewing matrix
    //XMMATRIX World;
    XMMATRIX View;
    XMMATRIX Projection;
    XMMATRIX Rotation;

    //camera vector
    XMVECTOR position;
    XMVECTOR target;
    XMVECTOR up;
    XMVECTOR right;
    XMVECTOR forward;
    XMVECTOR defaultRight;
    XMVECTOR defaultForward;

    //float rotation/direction
    float fYaw;
    float fPitch;
    float forwardDirection;
    float rightDirection;
    float upDirection;

    //functions
    bool init(ID3D11Device *device, float width, float height);
    void movement(INPUT_ *input, double dt);
    void destroy();
};

#endif