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
	//auto elem = getElementArea(area->_pos);
	//if (elem != _items.end()) {
	//	area->_zlevel = elem->get()->_zlevel  + 1;
	//}
	if (area->getType() == AREA_TYPE_FRAME) {
		auto frame = std::dynamic_pointer_cast<UIFrame>(area);
		auto subEl = frame->getElements();
		for (auto it = subEl.begin(); it != subEl.end(); it++) {
			_items.emplace(*it);
		}
	}
	_items.emplace(area);
}

std::unordered_set<shared_ptr<ScreenArea>>::iterator ScreenBuffer::getElementArea(const Point& pos) {
	int maxZ = 0;
	auto retval = _items.end();

	for (auto it = _items.begin(); it != _items.end(); it++) {
		auto it_ptr = it->get();
		if (pos.rectMoreThan(it_ptr->getPos()) && pos.rectLessThan(it_ptr->getMax())) {
			if (it_ptr->getType() == AREA_TYPE_OBJECT) {
				auto area = std::dynamic_pointer_cast<ObjectArea>(*it);

				auto sp = area->castSprite();
				if (sp && sp->checkAlpha(pos - area->getPos(), area->_obj->getSprite())) {
					if (area->getZlevel() > maxZ) {
						retval = it;
						maxZ = area->getZlevel();
					}
				}
			} else if (it_ptr->getType() == AREA_TYPE_ITEM) {
				auto area = std::dynamic_pointer_cast<ItemArea>(*it);

				auto sp = area->getSprite();
				if (sp && sp->checkAlpha(pos - area->getPos(), area->_item->getType())) {
					if (area->getZlevel() > maxZ) {
						cout << "Got item! " << area->_item->getID() << endl;
						retval = it;
						maxZ = area->getZlevel();
					}
				}
			} else {
				cout << (*it)->getType() << " hit!" << endl;
				if ((*it)->getZlevel() > maxZ) {
					retval = it;
					maxZ = (*it)->getZlevel();
				}
			}
		}
	}
	return retval;
}

shared_ptr<ScreenArea> ScreenBuffer::getElement(const Point& pos) {
	auto area = getElementArea(pos);
	if (area != _items.end()) {
		return *area;
	}
	return nullptr;
}

const std::unordered_set<shared_ptr<ScreenArea>>& ScreenBuffer::getAllAreas() const {
	return _items;
}

/*
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
*/