#pragma once
#include "main.h"
#include "Point.h"

#define MOUSE_BUTTON_LEFT 1
#define MOUSE_BUTTON_RIGHT 2
#define MOUSE_BUTTON_MIDDLE 3

class Mouse {
	Point _pos;
    int _frameid;
    Point _framePos;
    int _elementid;
    int _button;
    bool _pressed;
	int _spriteID;
public:
    Mouse();
    ~Mouse();
	bool set(const Point& abs, int button = 0, bool pressed = false, int elementID = -1);
	void setFrame(int frameid, const Point& framePos, int elementID = -1);
    int getXPos();
    int getYPos();
	Point getPos() const ;
    int getButton() const;
	int getSprite() const;
    bool isPressed() const;
    void setXPos(int x);
    void setYPos(int y);
	void setPos(const Point& pos);
    void setButton(int button);
	void setSprite(int id);
    void setPressed(bool pressed);
};