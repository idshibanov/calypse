#pragma once
#include "main.h"
#include "Point.h"

#define MOUSE_BUTTON_LEFT 1
#define MOUSE_BUTTON_RIGHT 2
#define MOUSE_BUTTON_MIDDLE 3

class Mouse {
	Point _pos;
	Point _lastPos;
    int _button;
    bool _pressed;
	int _spriteID;
public:
    Mouse();
    ~Mouse();
	bool set(const Point& abs, int button = 0, bool pressed = false);
	Point getDelta() const;
	Point getPos() const;
    int getButton() const;
	int getSprite() const;
    bool isPressed() const;
	void setPos(const Point& pos);
    void setButton(int button);	
	void setSprite(int id);
    void setPressed(bool pressed);
};