@echo off

g++ -o main ./src/main.cpp ./src/window.cpp ./src/d3d.cpp ./src/input.cpp ./src/texture.cpp ./src/rendering.cpp ./src/camera.cpp ./src/block.cpp ./src/d2dWrite.cpp ./src/timer.cpp ./src/chunk.cpp ./src/perlin.cpp -I../includes -I../third_library -ld3d11 -ld3dcompiler -ld2d1 -ldwrite

echo Compilation Done