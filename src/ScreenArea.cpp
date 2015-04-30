#include "ScreenArea.h"

ScreenArea::ScreenArea(const Point& pos, const Point& size) {
	_type = AREA_TYPE_SCREEN;
	_pos = pos;
	_size = size;
	_zlevel = 1;
}

ScreenArea::~ScreenArea() {

}

ScreenAreaType ScreenArea::getType() const {
	return _type;
}

Point ScreenArea::getPos() const {
	return _pos;
}

Point ScreenArea::getMax() const {
	return getPos() + _size;
}

int ScreenArea::getZlevel() const {
	return _zlevel;
}

bool ScreenArea::operator< (const ScreenArea& area) const {
	return getPos().toRenderPriority() < area.getPos().toRenderPriority();
}

bool ScreenArea::operator< (const Point& pos) const {
	return getPos().toRenderPriority() < pos.toRenderPriority();
}

bool ScreenArea::operator== (const ScreenArea& area) const {
	return getPos() == area.getPos() && _size == area._size;
}

size_t ScreenArea::operator() (const ScreenArea& area) const {
	return area.getPos().toRenderPriority();
}
