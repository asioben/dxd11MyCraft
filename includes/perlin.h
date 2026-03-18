#ifndef PERLIN_H
#define PERLIN_H

//2D map 
float perlin2D(float x, float z, float frequency, int seed);

//fractal map, create smoother 2d map with octave
float fractalPerlin2D(float x, float z, float frequency, float octave, float persistance, int seed);

#endif