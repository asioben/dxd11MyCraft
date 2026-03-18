#ifndef D2DWRITE_H
#define D2DWRITE_H

//include directx
#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
//include headers
#include "d3d.h"

using namespace D2D1;

struct TEXT{
    //pointer
    ID2D1Factory *d2dFactory = NULL;
    IDWriteFactory *dwriteFactory = NULL;
    ID2D1RenderTarget *d2dRenderTarget = NULL;
    ID2D1SolidColorBrush *d2dBrush = NULL;
    IDWriteTextFormat *dwriteTextFormat = NULL;

    //functions
    bool init(IDXGISwapChain *swapChain);
    bool renderText(const wchar_t *text, D2D1_RECT_F *rect);
    void destroy();
};

#endif