#include "ScreenBuffer.h"

ScreenBuffer::ScreenBuffer() {

}

ScreenBuffer::~ScreenBuffer() {

}

void ScreenBuffer::reset () {
	_items.clear();
}

void ScreenBuffer::setElement(ScreenArea area) {
	int sz = _items.size();
	// TODO: better algo for setting Z level
	auto elem = getElementArea(area._pos);
	if (elem != _items.end()) {
		area._zlevel = elem->_zlevel  + 1;
	}
	_items.emplace(area);
	if (_items.size() == sz) {
		sz++; // nope
	}
}

set<ScreenArea>::iterator ScreenBuffer::getElementArea(const Point& pos) {
	int maxZ = 0;
	set<ScreenArea>::iterator retval = _items.end();

	for (auto it = _items.begin(); it != _items.end(); it++) {
		if (pos > it->_pos && pos < it->getMax()) {
			auto sp = it->_sprite.lock();
			if (sp && sp->checkAlpha(pos - it->_pos)) {
				if (it->_zlevel > maxZ) {
					retval = it;
					maxZ = it->_zlevel;
				}
			}
		}
	}
	return retval;
}

shared_ptr<MapObject> ScreenBuffer::getElement(const Point& pos) {
	auto area = getElementArea(pos);
	if (area != _items.end()) {
		return getElement(*area);
	}
	return nullptr;
}

shared_ptr<MapObject> ScreenBuffer::getElement(const ScreenArea& area) {
	auto elem = _items.find(area);
	if (elem != _items.end()) {
		return elem->_obj;
	}
	return nullptr;
}