#include "mouse.h"

Mouse::Mouse() {
	_spriteID = 0;
    _button = 0;
    _pressed = false;
}

Mouse::~Mouse(){

}

bool Mouse::set(const Point& abs, int button, bool pressed) {
	bool retval = false;
	if (abs != _pos) {
		retval = true;
	}
	_lastPos = _pos;
    _pos = abs;
    _button = button;
    _pressed = pressed;
	return retval;
}

Point Mouse::getDelta() const {
	return _pos - _lastPos;
}

Point Mouse::getPos() const {
	return _pos;
}

int Mouse::getButton() const {
    return _button;
}

int Mouse::getSprite() const {
	return _spriteID;
}

bool Mouse::isPressed() const {
    return _pressed;
}

void Mouse::setPos(const Point& pos) {
	_pos = pos;
}

void Mouse::setButton(int button){
    _button = button;
}

void Mouse::setSprite(int id) {
	_spriteID = id;
}

void Mouse::setPressed(bool pressed){
    _pressed = pressed;
}
