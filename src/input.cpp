#include "../includes/input.h"
#include <iostream>

//void INPUT::_input(){}

USHORT INPUT_::keyboard_input(){
    if(!(keyboard.Flags & RI_KEY_BREAK))return keyboard.VKey;
    else return 0;
}

MOUSEINPUT_ INPUT_::mouse_input(){
    if(mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) return LEFT_DOWN;
    if(mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) return LEFT_UP;
    if(mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) return RIGHT_DOWN;
    if(mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) return RIGHT_UP;
    return DEFAULT;
}

POINT INPUT_::mouse_position(LONG *delta){
    POINT point = {0,0};
    if(mouse.lLastX != 0 && mouse.lLastY != 0 && delta != NULL){
        delta[0] = mouse.lLastX;
        delta[1] = mouse.lLastY;
    }
    if(!GetCursorPos(&point)){
        point = {0,0};
    }
    return point;
}

void INPUT_::destroy_input(){
    ZeroMemory(&keyboard,sizeof(keyboard));
    ZeroMemory(&mouse,sizeof(mouse));
}