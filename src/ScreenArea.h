#pragma once
#include "Sprite.h"

class ScreenArea {
public:
	Point _pos;
	Point _size;
	int _zlevel;
	weak_ptr<Sprite> _sprite;
	ScreenArea(const Point& pos, const Point& size, weak_ptr<Sprite> spr) {
		_pos = pos;
		_size = size;
		_zlevel = 1;
		_sprite = spr;
	}
	~ScreenArea() {}
	bool operator< (const ScreenArea& area) const {
		return _pos < area._pos;
	}
	bool operator< (const Point& pos) const {
		return _pos < pos;
	}
};