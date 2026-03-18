#include "../includes/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../third_library/stb_image.h"

bool TEXTURE::loadTexture(const char *filename, ID3D11Device *device){
    //load the image
    int width;
    int height;
    int channels;
    unsigned char* imgData = stbi_load(filename,&width,&height,&channels,STBI_rgb_alpha);

    if(!imgData) return error("Loading texture failed\n");

    //init the tex2d desc
    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc,sizeof(texDesc));
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;

    //init the data
    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData,sizeof(initData));
    initData.pSysMem = imgData;
    initData.SysMemPitch = width * 4;

    //create the texture and handle the error
    HRESULT hr = device->CreateTexture2D(&texDesc,&initData,&texture);
    if(FAILED(hr)){
        stbi_image_free(imgData);
        return error("Texture Creation failed\n");
    }

    //fill the shader resource view desc
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc,sizeof(srvDesc));
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    hr = device->CreateShaderResourceView(texture,&srvDesc,&textureView);
    stbi_image_free(imgData);
    if(FAILED(hr)) return error("Create Shader Resource View failed");

    return true;
}

bool TEXTURE::createTexture(const char *filename, ID3D11Device *device){
    //load the texture
    if(!loadTexture(filename,device)) return error("Loading Texture failed");

    //sampler desc struct
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc,sizeof(sampDesc));

    //filling it
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    //create the sampler
    if(FAILED(device->CreateSamplerState(&sampDesc,&samplerState))) return error("Sampler State Creation failed\n");

    return true;
}

SPRITE TEXTURE::getSprite(int index, int rows, int columns){
    SPRITE sprite;

    sprite.width = 1.0f / columns;
    sprite.height = 1.0f / rows; 
    sprite.u = (index % columns) * sprite.width;
    sprite.v = (index / columns) * sprite.height;

    return sprite;
}

void TEXTURE::destroyTexture(){
    SAFE_RELEASE(texture);
    SAFE_RELEASE(samplerState);
    SAFE_RELEASE(textureView);
}
