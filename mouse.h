#pragma once
#include "main.h"
#include "action.h"

#define TMOUSE_BUTTON_LEFT 1
#define TMOUSE_BUTTON_RIGHT 2
#define TMOUSE_BUTTON_MIDDLE 3

class TMouse {
    int _xpos;
    int _ypos;
    int _frameid;
    int _framex;
    int _framey;
    int _elementid;
    int _button;
    TMouseSpecialAction _specialAction;
    bool _pressed;
public:
    TMouse();
    ~TMouse();
    void set(int absX, int absY, int button = 0, bool pressed = false, int frameid = -1, 
             int frameX = 0, int frameY = 0, int elementID = -1);
    void setFrame(int frameid, int frameX, int frameY, int elementID = -1);
    int getXPos();
    int getYPos();
    int getButton();
    bool isPressed();
    TMouseSpecialAction getSpecialAction();
    void setXPos(int x);
    void setYPos(int y);
    void setButton(int button);
    void setPressed(bool pressed);
    void setSpecialAction(TMouseSpecialAction action);
};