#include "ScreenArea.h"

ScreenArea::ScreenArea(const Point& pos, const Point& size, shared_ptr<MapObject> obj, weak_ptr<Sprite> spr) {
	_pos = pos;
	_size = size;
	_zlevel = 1;
	_obj = obj;
	_sprite = spr;
}

ScreenArea::~ScreenArea() {

}

bool ScreenArea::operator< (const ScreenArea& area) const {
	return _pos < area._pos;
}

bool ScreenArea::operator< (const Point& pos) const {
	return _pos < pos;
}