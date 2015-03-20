#include "ScreenBuffer.h"

ScreenBuffer::ScreenBuffer() {

}

ScreenBuffer::~ScreenBuffer() {

}

void ScreenBuffer::reset () {
	_items.clear();
}

void ScreenBuffer::setElement(shared_ptr<ScreenArea> area) {
	// TODO: better algo for setting Z level
	auto elem = getElementArea(area->_pos);
	if (elem != _items.end()) {
		area->_zlevel = elem->get()->_zlevel  + 1;
	}
	_items.emplace(area);
}

unordered_set<shared_ptr<ScreenArea>>::iterator ScreenBuffer::getElementArea(const Point& pos) {
	int maxZ = 0;
	auto retval = _items.end();

	for (auto it = _items.begin(); it != _items.end(); it++) {
		auto area = dynamic_pointer_cast<ObjectArea>(*it);
		if (pos > area->_pos && pos < area->getMax()) {
			auto sp = area->_sprite;
			if (sp && sp->checkAlpha(pos - area->_pos, area->_obj->getSprite())) {
				if (area->_zlevel > maxZ) {
					retval = it;
					maxZ = area->_zlevel;
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

shared_ptr<MapObject> ScreenBuffer::getElement(shared_ptr<ScreenArea> area) {
	auto elem = _items.find(area);
	if (elem != _items.end()) {
		auto objArea = dynamic_pointer_cast<ObjectArea>(*elem);
		return objArea->_obj;
	}
	return nullptr;
}