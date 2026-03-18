#ifndef TIMER_H
#define TIMER_H

//headers
#include "d3d.h"

struct TIME{
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;

    void initTime();
    double deltaTime();
    bool counter(double delta);
};

#endif