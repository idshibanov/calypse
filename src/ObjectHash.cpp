#include <functional>
#include "ObjectHash.h"

ObjectHash::ObjectHash() {
	
}

ObjectHash::~ObjectHash() {

}

bool ObjectHash::setObject(const Point& pos, const Point& size, shared_ptr<MapObject> obj) {
	bool retval = false;
	Rect area(pos, size);
	int id = obj->getID();

	std::function<bool(const Point&)> search = [this](const Point& pos) {
		return isFree(pos);
	};

	std::function<void(const Point&)> setter = [this, &id](const Point& pos) {
		_mask.emplace(pos.toID(_size), id);
	};

	if (area.iterate(search, true)) {
		_objects.emplace(id, obj);

		area.iterate(setter);
		retval = true;
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