#ifndef INPUT_H
#define INPUT_H

//include headers
#include <windows.h>

enum MOUSEINPUT_{
    LEFT_DOWN,
    LEFT_UP,
    RIGHT_DOWN,
    RIGHT_UP,
    DEFAULT
};

//struct input
struct INPUT_{
    public:
    //data
    RAWKEYBOARD keyboard;
    RAWMOUSE mouse;

    public:
    //functions
    USHORT keyboard_input();
    MOUSEINPUT_ mouse_input();
    POINT mouse_position(LONG *delta);
    void destroy_input();
};

#endif