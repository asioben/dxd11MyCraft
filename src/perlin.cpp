#include "../includes/perlin.h"

#define STB_PERLIN_IMPLEMENTATION
#include "../third_library/stb_perlin.h"

float perlin2D(float x, float z, float frequency, int seed){
    return stb_perlin_noise3(x*frequency,0,z*frequency,seed,0,0);
}

float fractalPerlin2D(float x, float z, float frequency, float octave, float persistance, int seed){
    float total = 0.0f;
    float amplitude = 1.0f;
    float max = 0.0f;

    for(int i = 0; i < octave; i++){
        total += perlin2D(x,z,frequency,seed) * amplitude;
        max += amplitude;
        amplitude *= persistance;
        frequency *= 2.0f;
    }
    return total/max;
}