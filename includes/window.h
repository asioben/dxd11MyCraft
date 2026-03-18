#ifndef WINDOW_H
#define WINDOW_H

//windows header
#define WIN32_LEAN_AND_MEAN
#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

//headers
#include "d3d.h"
#include "input.h"
#include "rendering.h"
#include "texture.h"
#include "d2dWrite.h"
#include "timer.h"
#include "chunk.h"

//standard library
#include <sstream>

//Window struct
struct WINDOW{
  public:
  //data
  HWND hWnd = NULL;
  HINSTANCE hInstance;
  LPCWSTR name;

  UINT width;
  UINT height;
  UINT posX;
  UINT posY;

  D3D *d3d;
  INPUT_ *input;
  VOXEL *cube;
  TEXTURE *texture;
  CAMERA *camera;
  TEXT *text;
  TIME *time;
  CHUNKMANAGER *chunk_;

  bool fullscreen;

  public:
  //function
  bool init();
  void update();
  void destroy();

  private:
  bool initWindow(WNDCLASSEX &wc);
  bool fullScreen(WNDCLASSEX &wc);
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif