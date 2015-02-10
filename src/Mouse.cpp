#include "mouse.h"

Mouse::Mouse() {
    _frameid = -1;
    _elementid = -1;
    _button = 0;
    _pressed = false;
}

Mouse::~Mouse(){

}

bool Mouse::set(const Point& abs, int button, bool pressed, int elementID) {
	bool retval = false;
	if (abs != _pos) {
		retval = true;
	}
    _pos = abs;
    _button = button;
    _pressed = pressed;
    _elementid = elementID;
	return retval;
}

void Mouse::setFrame(int frameid, const Point& framePos, int elementID) {
    _frameid = frameid;
    _framePos = framePos;
    _elementid = elementID;
}

int Mouse::getXPos(){
    return _pos._x;
}

int Mouse::getYPos(){
    return _pos._y;
}

Point Mouse::getPos() {
	return _pos;
}

int Mouse::getButton(){
    return _button;
}

bool Mouse::isPressed(){
    return _pressed;
}

void Mouse::setXPos(int x){
    _pos._x = x;
}

void Mouse::setYPos(int y){
    _pos._y = y;
}

void Mouse::setPos(const Point& pos) {
	_pos = pos;
}

void Mouse::setButton(int button){
    _button = button;
}

void Mouse::setPressed(bool pressed){
    _pressed = pressed;
}
