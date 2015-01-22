#include "mouse.h"

Mouse::Mouse() {
    _xpos = 0;
    _ypos = 0;
    _frameid = -1;
    _framex = 0;
    _framey = 0;
    _elementid = -1;
    _button = 0;
    _pressed = false;
}

Mouse::~Mouse(){

}

bool Mouse::set(int absX, int absY, int button, bool pressed, int frameid, 
                  int frameX, int frameY, int elementID) {
	bool retval = false;
	if (absX != _xpos || absY != _ypos) {
		retval = true;
	}
    _xpos = absX;
    _ypos = absY;
    _button = button;
    _pressed = pressed;
    _frameid = frameid;
    _framex = frameX;
    _framey = frameY;
    _elementid = elementID;
	return retval;
}

void Mouse::setFrame(int frameid, int frameX, int frameY, int elementID) {
    _frameid = frameid;
    _framex = frameX;
    _framey = frameY;
    _elementid = elementID;
}

int Mouse::getXPos(){
    return _xpos;
}

int Mouse::getYPos(){
    return _ypos;
}

int Mouse::getButton(){
    return _button;
}

bool Mouse::isPressed(){
    return _pressed;
}

void Mouse::setXPos(int x){
    _xpos = x;
}

void Mouse::setYPos(int y){
    _ypos = y;
}

void Mouse::setButton(int button){
    _button = button;
}

void Mouse::setPressed(bool pressed){
    _pressed = pressed;
}
