#include "../includes/timer.h"

void TIME::initTime(){
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
}

double TIME::deltaTime(){
    QueryPerformanceCounter(&end);
    double deltaTime_ = static_cast<double>(end.QuadPart-start.QuadPart)/frequency.QuadPart;
    return deltaTime_;
}

bool TIME::counter(double delta){
    if(delta <= deltaTime()){
        start = end;
        return true;
    }
    else{return false;}
}