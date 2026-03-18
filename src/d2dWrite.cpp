#include "../includes/d2dWrite.h"

bool TEXT::init(IDXGISwapChain *swapChain){
    //get dxgi surface from back buffer
    IDXGISurface *dxgiSurface;
    if(FAILED(swapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&dxgiSurface)))return error("Getting back buffer failed\n");

    //creat d2d factory
    if(FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory))) printf("Create d2d factory failed\n");

    //create and fill render target properties
    D2D1_RENDER_TARGET_PROPERTIES rtProps = RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),0.0f,0.0f);

    //create d2d render target
    HRESULT hr = d2dFactory->CreateDxgiSurfaceRenderTarget(dxgiSurface,&rtProps,&d2dRenderTarget);
    if(FAILED(hr)) return error("Create d2d render target failed\n");
    dxgiSurface->Release();

    //create solid color brush
    if(FAILED(d2dRenderTarget->CreateSolidColorBrush(ColorF(ColorF::White), &d2dBrush))) printf("Create solid color brush failed\n");

    //create directwrite factory
    if(FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&dwriteFactory))) printf("Create direct write factory failed\n");

    //create text format
    if(FAILED(dwriteFactory->CreateTextFormat(L"Segoe UI",NULL,DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,24.0f,L"en-us",&dwriteTextFormat))) printf("Create text format failed\n");

    return true;
}

bool TEXT::renderText(const wchar_t *text, D2D1_RECT_F *rect){
    //draw on the d2d render target (clear it too)
    d2dRenderTarget->BeginDraw();
    d2dRenderTarget->SetTransform(Matrix3x2F::Identity());

    //actual drawing(rendering text)
    d2dRenderTarget->DrawText(text,wcslen(text),dwriteTextFormat,rect,d2dBrush);

    HRESULT hr = d2dRenderTarget->EndDraw();
    if(FAILED(hr)) error("Directwrite failed\n");

    return true;
}

void TEXT::destroy(){
    SAFE_RELEASE(d2dBrush);
    SAFE_RELEASE(d2dFactory);
    SAFE_RELEASE(d2dRenderTarget);
    SAFE_RELEASE(dwriteFactory);
    SAFE_RELEASE(dwriteTextFormat);
}