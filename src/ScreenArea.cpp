#include "ScreenArea.h"

ScreenArea::ScreenArea(const Point& pos, const Point& size, shared_ptr<MapObject> obj, SpriteSheet* spr) {
	_pos = pos;
	_size = size;
	_zlevel = 1;
	_obj = obj;
	_sprite = spr;
}

ScreenArea::~ScreenArea() {

}

Point ScreenArea::getMax() const {
	return _pos + _size;
}

bool ScreenArea::operator< (const ScreenArea& area) const {
	return _pos.toRenderPriority() < area._pos.toRenderPriority();
}

bool ScreenArea::operator< (const Point& pos) const {
	return _pos.toRenderPriority() < pos.toRenderPriority();
}

size_t ScreenArea::operator() (const ScreenArea& area) const {
	return area._pos.toRenderPriority();
}
