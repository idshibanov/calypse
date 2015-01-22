#pragma once
#include "main.h"

#define MOUSE_BUTTON_LEFT 1
#define MOUSE_BUTTON_RIGHT 2
#define MOUSE_BUTTON_MIDDLE 3

class Mouse {
    int _xpos;
    int _ypos;
    int _frameid;
    int _framex;
    int _framey;
    int _elementid;
    int _button;
    bool _pressed;
public:
    Mouse();
    ~Mouse();
    bool set(int absX, int absY, int button = 0, bool pressed = false, int frameid = -1, 
             int frameX = 0, int frameY = 0, int elementID = -1);
    void setFrame(int frameid, int frameX, int frameY, int elementID = -1);
    int getXPos();
    int getYPos();
    int getButton();
    bool isPressed();
    void setXPos(int x);
    void setYPos(int y);
    void setButton(int button);
    void setPressed(bool pressed);
};