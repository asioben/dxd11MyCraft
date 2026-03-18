#include "../includes/rendering.h"

bool initVertexBuffer(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ID3D11Buffer *&vertices, ID3D11Buffer *&indices, const void *vertex, int *indice, size_t size, int indices_size){
    //the buffer struct desc
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd,sizeof(bd));

    //fill the struct
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = size;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    //create the buffer
    if(FAILED(device->CreateBuffer(&bd,NULL,&vertices))) return error("Vertex Buffer Creation failed \n");

    //copy the triangles (vertex) vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    deviceContext->Map(vertices,0,D3D11_MAP_WRITE_DISCARD,0,&ms);
    memcpy(ms.pData,vertex,size);
    deviceContext->Unmap(vertices,0);

    //now the indexes
    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd,sizeof(D3D11_BUFFER_DESC));

    //fill it
    ibd.Usage = D3D11_USAGE_DYNAMIC;
    ibd.ByteWidth = sizeof(int) * indices_size;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    //create the buffer and set it
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = indice;
    if(FAILED(device->CreateBuffer(&ibd,&initData,&indices))) return error("Indices Buffer Creation failed\n");
    deviceContext->IASetIndexBuffer(indices,DXGI_FORMAT_R32_UINT,0);

    return true;
}

bool VOXEL::initDrawing(ID3D11Device *device, ID3D11DeviceContext *deviceContext, TEXTURE *texture, CHUNKMANAGER *chunks){
    if(!chunks->chunks[chunks->currentChunk].chunkUpdated) return true;

    VERTEX *face = new VERTEX[chunks->faces.size()];
    int *indice = new int[chunks->indices.size()];

    for(int i = 0; i < chunks->faces.size(); i++) face[i] = chunks->faces[i];
    for(int j = 0; j < chunks->indices.size(); j++) indice[j] = chunks->indices[j];

    /*//Triangles
    VERTEX cube[24] = {
        // Front face
        {{0.0f, 0.0f, 0.0f},{0.0f, 1.0f}},
        {{0.0f,  1.0f, 0.0f},{0.0f, 0.0f}},
        {{ 1.0f,  1.0f, 0.0f},{1.0f, 0.0f}},
        {{ 1.0f, 0.0f, 0.0f},{1.0f, 1.0f}},
    
        // Back face
        {{0.0f, 0.0f, 1.0f},{1.0f, 1.0f}},
        {{ 1.0f, 0.0f, 1.0f},{0.0f, 1.0f}},
        {{ 1.0f,  1.0f, 1.0f},{0.0f, 0.0f}},
        {{0.0f,  1.0f, 1.0f},{1.0f, 0.0f}},
    
        // Top face
        {{0.0f, 1.0f, 0.0f},{0.0f, 1.0f}},
        {{0.0f, 1.0f,  1.0f},{0.0f, 0.0f}},
        {{ 1.0f, 1.0f,  1.0f},{1.0f, 0.0f}},
        {{ 1.0f, 1.0f, 0.0f},{1.0f, 1.0f}},
    
        // Bottom face
        {{0.0f, 0.0f, 0.0f},{1.0f, 1.0f}},
        {{ 1.0f, 0.0f, 0.0f},{0.0f, 1.0f}},
        {{ 1.0f, 0.0f,  1.0f},{0.0f, 0.0f}},
        {{0.0f, 0.0f,  1.0f},{1.0f, 0.0f}},
    
        // Left face
        {{0.0f, 0.0f,  1.0f},{0.0f, 1.0f}},
        {{0.0f,  1.0f,  1.0f},{0.0f, 0.0f}},
        {{0.0f,  1.0f, 0.0f},{1.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f},{1.0f, 1.0f}},
    
        // Right face
        {{1.0f, 0.0f, 0.0f},{0.0f, 1.0f}},
        {{1.0f,  1.0f, 0.0f},{0.0f, 0.0f}},
        {{1.0f,  1.0f,  1.0f},{1.0f, 0.0f}},
        {{1.0f, 0.0f,  1.0f},{1.0f, 1.0f}},
    };
    
    int indice[36] = {
        //front face
        0,1,2,
        0,2,3,
        //back face
        4,5,6,
        4,6,7,
        //top face
        8,9,10,
        8,10,11,
        //bottom face
        12,13,14,
        12,14,15,
        //left face
        16,17,18,
        16,18,19,
        //right face
        20,21,22,
        20,22,23
    };*/

    if(!initVertexBuffer(device,deviceContext,vertices,indices,face,indice,sizeof(VERTEX)*chunks->faces.size(),chunks->indices.size())) return false;

    if(!initInstances){

        //select the primitive topology we'll use
        deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        
        //texture
        deviceContext->PSSetShaderResources(0,1,&texture->textureView);
        deviceContext->PSSetSamplers(0,1,&texture->samplerState); 

        initInstances = true;
    }

    chunks->toRender[chunks->currentChunk].chunkUpdated = false;

    delete[] face;
    delete[] indice;

    return true;
}

void VOXEL::drawCube(D3D *d3d, CHUNKMANAGER *chunks, TEXTURE *texture, CAMERA *camera){
    initDrawing(d3d->device,d3d->deviceContext,texture,chunks);

    //select the vertex buffer to display
    ID3D11Buffer *buffers = vertices;
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    d3d->deviceContext->IASetVertexBuffers(0,1,&buffers,&stride,&offset);

    //matrixes operation
    camera->cb.VP = XMMatrixTranslation(1.0f,1.0f,1.0f) * camera->View * camera->Projection;
    camera->cb.VP = XMMatrixTranspose(camera->cb.VP);

    //update the buffer
    d3d->deviceContext->UpdateSubresource(camera->constantBuffer,0,NULL,&camera->cb,0,0);

    //set the buffer to the vertex shader constant buffer
    d3d->deviceContext->VSSetConstantBuffers(0,1,&camera->constantBuffer);
    //printf("%i",chunks->indices.size());
    d3d->deviceContext->DrawIndexed(chunks->indices.size(),0,0);
   
}

void VOXEL::destroyCube(){
    SAFE_RELEASE(vertices);
    SAFE_RELEASE(indices);
    SAFE_RELEASE(instancesBuffer);
    SAFE_RELEASE(texturesBuffer);
}