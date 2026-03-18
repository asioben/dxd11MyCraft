#include "../includes/camera.h"

bool CAMERA::init(ID3D11Device *device, float width, float height){
    //create a buffer
    D3D11_BUFFER_DESC cbDesc;
    ZeroMemory(&cbDesc,sizeof(D3D11_BUFFER_DESC));

    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.ByteWidth =  sizeof(cb);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = 0;
    cbDesc.MiscFlags = 0;

    if(FAILED(device->CreateBuffer(&cbDesc,NULL,&constantBuffer))) return error("Constant Buffer Creation failed");

    //set the camera vector
    position = XMVectorSet(8.0f,16.0f,8.0f,0.0f);
    target = XMVectorSet(0.0f,0.0f,1.0f,0.0f);
    up = XMVectorSet(0.0f,1.0f,0.0f,0.0f);
    right = XMVectorSet(1.0f,0.0f,0.0f,0.0f);
    forward = XMVectorSet(0.0f,0.0f,1.0f,0.0f);
    defaultForward = forward;
    defaultRight = right;

    fYaw = 0.0f;
    fPitch = 0.0f;
    rightDirection = 0.0f;
    forwardDirection = 0.0f;
    upDirection = 0.0f;

    //create matrixes
    //World = XMMatrixIdentity();
    View = XMMatrixLookAtLH(position,target,up);
    Projection = XMMatrixPerspectiveFovLH(0.4f*0.5f*XM_2PI,width/height,0.1f,1000.0f);

    return true;
}

void CAMERA::movement(INPUT_ *input, double dt){
    //storing mouse movement
    LONG delta[2] = {0,0};

    //keyboard input
    if(input->keyboard_input() == VK_UP) forwardDirection = 10.0f;
    if(input->keyboard_input() == VK_DOWN) forwardDirection = -10.0f;
    if(input->keyboard_input() == VK_RIGHT) rightDirection = 10.0f;
    if(input->keyboard_input() == VK_LEFT) rightDirection = -10.0f;
    if(input->keyboard_input() == 'W') upDirection = 10.0f;
    if(input->keyboard_input() == 'S') upDirection = -10.0f;

    //mouse input
    input->mouse_position(delta);
    fYaw += delta[0] * 0.00075f;
    fPitch += delta[1] * 0.001f;
    if(fYaw >= XM_2PI) fYaw = 0.0f;
    if(fPitch >= XM_2PI/4 )fPitch = (XM_2PI/4)-0.001f; 
    if(fPitch <= -XM_2PI/4)fPitch = (-XM_2PI/4)+0.001f;

    //axis
    //World = XMMatrixIdentity();

    //update position
    Rotation = XMMatrixRotationRollPitchYaw(fPitch,fYaw,0);
    target = XMVector3TransformCoord(defaultForward,Rotation);
    target = XMVector3Normalize(target);
    XMMATRIX RotationY = XMMatrixRotationY(fYaw);
    right = XMVector3TransformCoord(defaultRight,RotationY);
    up = XMVector3TransformCoord(up,RotationY);
    forward = XMVector3TransformCoord(defaultForward,RotationY);
    forwardDirection *= dt;
    rightDirection *= dt;
    upDirection *= dt;
    position += forwardDirection * forward;
    position += rightDirection * right;
    position += upDirection * up;
    forwardDirection = 0.0f;
    rightDirection = 0.0f;
    upDirection = 0.0f;
    target = position + target;
    View = XMMatrixLookAtLH(position,target,up);

}

void CAMERA::destroy(){
    SAFE_RELEASE(constantBuffer);
}