#include "../includes/skybox.h"

bool SKYBOX::init(ID3D11Device *device, ID3D11DeviceContext *deviceContext, const WCHAR *vFile, const WCHAR *pFile, const char *vFunction, const char *pFunction){
    SPECIAL_VERTEX cube[] ={
        {{ -1.0f,  1.0f, -1.0f }, { (130.0f/255.0f),  (171.0f/255.0f), (254.0f/255.0f), (1.0f) }}, 
        // 0: top-left-back
        {{  1.0f,  1.0f, -1.0f }, { (130.0f/255.0f),  (171.0f/255.0f), (254.0f/255.0f), (1.0f) }},
        // 1: top-right-back
        {{  1.0f, -1.0f, -1.0f }, { (130.0f/255.0f),  (171.0f/255.0f), (254.0f/255.0f), (1.0f) }}, 
        // 2: bottom-right-back
        {{ -1.0f, -1.0f, -1.0f }, { (130.0f/255.0f),  (171.0f/255.0f), (254.0f/255.0f), (1.0f) }}, 
        // 3: bottom-left-back
        {{ -1.0f,  1.0f,  1.0f }, { (130.0f/255.0f),  (171.0f/255.0f), (254.0f/255.0f), (1.0f) }}, 
        // 4: top-left-front
        {{  1.0f,  1.0f,  1.0f }, { (130.0f/255.0f),  (171.0f/255.0f), (254.0f/255.0f), (1.0f) }}, 
        // 5: top-right-front
        {{  1.0f, -1.0f,  1.0f }, { (130.0f/255.0f),  (171.0f/255.0f), (254.0f/255.0f), (1.0f) }},
        // 6: bottom-right-front
        {{ -1.0f, -1.0f,  1.0f }, { (130.0f/255.0f),  (171.0f/255.0f), (254.0f/255.0f), (1.0f) }}, 
        // 7: bottom-left-front
    };
    

    short indice[36] = {
        /// Back face
        0, 2, 1,
        0, 3, 2,

        // Front face
        4, 5, 6,
        4, 6, 7,

        // Left face
        4, 7, 3,
        4, 3, 0,

        // Right face
        1, 2, 6,
        1, 6, 5,

        // Top face
        4, 0, 1,
        4, 1, 5,

        // Bottom face
        2, 3, 7,
        2, 7, 6
    };

    D3D11_INPUT_ELEMENT_DESC ied[2] = {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
    };

    initShaders(device,deviceContext,vertexShader,pixelShader,inputLayout,vFile,pFile,vFunction,pFunction,ied,2);

    //if(!initVertexBuffer(device,deviceContext,vertices,indexes,cube,indice,sizeof(SPECIAL_VERTEX)*8)) return false;

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

    //new struct
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory(&dsDesc,sizeof(D3D11_DEPTH_STENCIL_DESC));

    //depth test parameters
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    //create depth stencil state
    if(FAILED(device->CreateDepthStencilState(&dsDesc,&depthStencilState))) return error("Depth Stencil State creation failed\n");

    if(FAILED(device->CreateTexture2D(&depthStencilDesc,NULL,&depthStencilBuffer)))return error("Depth Stencil View texture/buffer creation failed\n");

    //struct desc about rasterizer state
    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory(&rasterDesc,sizeof(D3D11_RASTERIZER_DESC));

    //fill this struct desc
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.DepthClipEnable = true;
    
    //create the rasterizer state
    if(FAILED(device->CreateRasterizerState(&rasterDesc,&rasterizerState))) return error("Rasterizer State Creation failed\n");

    return true;
}

bool SKYBOX::initDrawing(ID3D11DeviceContext *deviceContext){
    //shader and input layout
    deviceContext->VSSetShader(vertexShader,NULL,0);
    deviceContext->PSSetShader(pixelShader,NULL,0);
    deviceContext->IASetInputLayout(inputLayout);

    //set the depth stencil state 
    deviceContext->OMSetDepthStencilState(depthStencilState,0);

    //set it
    deviceContext->RSSetState(rasterizerState);

    return true;
}

void SKYBOX::draw(ID3D11Device *device, ID3D11DeviceContext *deviceContext, CAMERA *camera){
    //pre drawing
    initDrawing(deviceContext);

    //select the vertex buffer to display
    UINT stride = sizeof(SPECIAL_VERTEX);
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0,1,&vertices,&stride,&offset);
    
    //select the primitive topology we'll use
    deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //matrixes operation
    XMMATRIX View = camera->View;
    View.r[3] = XMVectorSet(0.0f,0.0f,0.0f,1.0f);
    camera->cb.VP = View * camera->Projection;
    camera->cb.VP = XMMatrixTranspose(camera->cb.VP);

    //update the buffer
    deviceContext->UpdateSubresource(camera->constantBuffer,0,NULL,&camera->cb,0,0);

    //set the buffer to the vertex shader constant buffer
    deviceContext->VSSetConstantBuffers(0,1,&camera->constantBuffer);

    //draw 
    deviceContext->DrawIndexed(36,0,0);
}

void SKYBOX::destroy(){
    SAFE_RELEASE(vertices);
    SAFE_RELEASE(indexes);
    SAFE_RELEASE(depthStencilState);
    SAFE_RELEASE(rasterizerState);
    SAFE_RELEASE(vertexShader);
    SAFE_RELEASE(pixelShader);
    SAFE_RELEASE(inputLayout);
    SAFE_RELEASE(depthStencilBuffer);
}