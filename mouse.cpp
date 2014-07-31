#include "mouse.h"

TMouse::TMouse() {
    _xpos = 0;
    _ypos = 0;
    _frameid = -1;
    _framex = 0;
    _framey = 0;
    _elementid = -1;
    _button = 0;
    _pressed = false;
    _specialAction = TMOUSE_SPECIAL_ACTION_NONE;
}

TMouse::~TMouse(){

}
void TMouse::set(int absX, int absY, int button, bool pressed, int frameid, 
                  int frameX, int frameY, int elementID) {
    _xpos = absX;
    _ypos = absY;
    _button = button;
    _pressed = pressed;
    _frameid = frameid;
    _framex = frameX;
    _framey = frameY;
    _elementid = elementID;
}

void TMouse::setFrame(int frameid, int frameX, int frameY, int elementID) {
    _frameid = frameid;
    _framex = frameX;
    _framey = frameY;
    _elementid = elementID;
}

int TMouse::getXPos(){
    return _xpos;
}

int TMouse::getYPos(){
    return _ypos;
}

int TMouse::getButton(){
    return _button;
}

bool TMouse::isPressed(){
    return _pressed;
}

TMouseSpecialAction TMouse::getSpecialAction(){
    return _specialAction;
}

void TMouse::setXPos(int x){
    _xpos = x;
}

void TMouse::setYPos(int y){
    _ypos = y;
}

void TMouse::setButton(int button){
    _button = button;
}

void TMouse::setPressed(bool pressed){
    _pressed = pressed;
}

void TMouse::setSpecialAction(TMouseSpecialAction action){
    _specialAction = action;
}
