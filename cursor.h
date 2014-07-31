#pragma once
#include "main.h"

class TCursor {
    unsigned _xpos;
    unsigned _ypos;
public:
    TCursor(unsigned x, unsigned y);
    ~TCursor();
    unsigned getXPos();
    unsigned getYPos();
    void setPos(unsigned x, unsigned y);
    void decreaseX();
    void decreaseY();
    void increaseX();
    void increaseY();
};