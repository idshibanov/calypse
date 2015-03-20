#include "ScreenArea.h"

ScreenArea::ScreenArea(const Point& pos, const Point& size) {
	_pos = pos;
	_size = size;
	_zlevel = 1;
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

bool ScreenArea::operator== (const ScreenArea& area) const {
	return _pos == area._pos && _size == area._size;
}

size_t ScreenArea::operator() (const ScreenArea& area) const {
	return area._pos.toRenderPriority();
}
