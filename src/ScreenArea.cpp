#include "ScreenArea.h"

ObjectArea::ObjectArea(const Point& pos, const Point& size, shared_ptr<MapObject> obj, SpriteSheet* spr) {
	_pos = pos;
	_size = size;
	_zlevel = 1;
	_obj = obj;
	_sprite = spr;
}

ObjectArea::~ObjectArea() {

}

Point ObjectArea::getMax() const {
	return _pos + _size;
}

bool ObjectArea::operator< (const ObjectArea& area) const {
	return _pos.toRenderPriority() < area._pos.toRenderPriority();
}

bool ObjectArea::operator< (const Point& pos) const {
	return _pos.toRenderPriority() < pos.toRenderPriority();
}

bool ObjectArea::operator== (const ObjectArea& area) const {
	return _pos == area._pos && _size == area._size;
}

size_t ObjectArea::operator() (const ObjectArea& area) const {
	return area._pos.toRenderPriority();
}
