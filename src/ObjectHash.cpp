#include "ObjectHash.h"

ObjectHash::ObjectHash() {

}

ObjectHash::~ObjectHash() {

}

bool ObjectHash::setObject(int id, const Point& pos, const Point& size) {
	bool retval = true;

	Point last = pos + size;
	for (int x = pos._x; x < last._x; x++) {
		for (int y = pos._y; y < last._y; y++) {
			if (!isFree(Point(x, y))) {
				retval = false;
			}
		}
	}

	if (retval) {
		//_objects.insert();
		for (int x = pos._x; x < last._x; x++) {
			for (int y = pos._y; y < last._y; y++) {
				_mask.insert(y*_size+x,id);
			}
		}
	}
	return retval;
}

bool ObjectHash::isFree(const Point& pos) {
	if (_mask.find(pos.toID(_size)) != _mask.end()) {
		return true;
	}
	return false;
}

map<int, shared_ptr<MapObject>> ObjectHash::getObjects(const Point& first, const Point& last) {
	map<int, shared_ptr<MapObject>> retval;
	Point size = last - first;

	for (auto obj : _objects) {
		Point pos = obj.second->getPos();

		if (pos > first * TILE_MASK && pos < last * TILE_MASK) {
			retval.insert(obj);
		}
	}
	return retval;
}