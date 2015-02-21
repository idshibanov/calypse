#include "ScreenBuffer.h"

ScreenBuffer::ScreenBuffer() {

}

ScreenBuffer::~ScreenBuffer() {

}

void ScreenBuffer::reset () {
	_items.clear();
}

void ScreenBuffer::setElement(ScreenArea area, shared_ptr<MapObject> obj) {
	auto elem = getElementArea(area._pos);
	if (elem) {
		area._zlevel = elem->_zlevel  + 1;
	}
	_items.emplace(area, obj);
}

const shared_ptr<ScreenArea> ScreenBuffer::getElementArea(const Point& pos) {
	int maxZ = 0;
	shared_ptr<ScreenArea> ptr = nullptr;
	for (auto item : _items) {
		if (pos < item.first._pos) {
			if (item.first._zlevel > maxZ) {
				maxZ = item.first._zlevel;
			}
		}
	}
	return ptr;
}

shared_ptr<MapObject> ScreenBuffer::getElement(const Point& pos) {
	auto area = getElementArea(pos);
	if (area) {
		return getElement(*area);
	}
	return nullptr;
}

shared_ptr<MapObject> ScreenBuffer::getElement(const ScreenArea& area) {
	auto elem = _items.find(area);
	if (elem != _items.end()) {
		return elem->second;
	}
	return nullptr;
}