#include <functional>
#include "ObjectHash.h"


ObjectHash::ObjectHash(shared_ptr<ResourceCtl> res, int size) {
	_res = res;
	_size = size;
}


ObjectHash::~ObjectHash() {

}


bool ObjectHash::setObject(shared_ptr<MapObject> obj) {
	auto objInfo = _res->getObjectInfo(obj->getType());
	Rect area(obj->getPos(), objInfo->mapSize());
	int id = obj->getID();

	if (obj->getType() == 0) {
		auto act = std::dynamic_pointer_cast<Actor>(obj);
		_actors.push_back(act);
	}

	std::function<bool(const Point&)> search = [this](const Point& pos) {
		return isFree(pos);
	};

	std::function<void(const Point&)> setter = [this, &id](const Point& pos) {
		_mask.emplace(pos.toID(_size), id);
	};

	if (area.iterate(search, true)) {
		area.iterate(setter);
		_objects.emplace(id, obj);
		return true;
	}
	return false;
}


bool ObjectHash::setObject(const Point& pos, shared_ptr<MapObject> obj) {
	int id = obj->getID();
	_objects.emplace(id, obj);
	return true;
}


bool ObjectHash::toggleObject(const Point& pos) {
	auto obj = getObject(pos);
	if (obj != nullptr) {
		auto objInfo = _res->getObjectInfo(obj->getType());
		Rect area(obj->getPos(), objInfo->mapSize());

		if (obj->getType() == 1) {
			if (obj->getSprite() == 1) {
				std::function<void(const Point&)> resetter = [this](const Point& pos) {
					_mask.erase(pos.toID(_size));
				};

				area.iterate(resetter);
				_objects.erase(obj->getID());
			} else {
				obj->setSprite(1);
			}
		}
		return true;
	}
	return false;
}


bool ObjectHash::resetObject(const Point& pos) {
	// search object map (not mask)
	// reset pos by obj pos + size from resource
	return false;
}


bool ObjectHash::isFree(const Point& pos) const {
	return checkPos(pos) == OBJECT_NOT_FOUND;
}


int ObjectHash::checkPos(const Point& pos) const {
	auto it = _mask.find(pos.toID(_size));
	if (it != _mask.end()) {
		return it->second;
	}
	return OBJECT_NOT_FOUND;
}


std::vector<Point> ObjectHash::searchForObject(const Point& pos) {
	std::vector<Point> retval;

	int objID = checkPos(pos);
	if (objID != OBJECT_NOT_FOUND) {
		retval.push_back(pos);

	}
	return retval;
}


shared_ptr<MapObject> ObjectHash::getObject(int objID) {
	auto it = _objects.find(objID);
	if (it != _objects.end()) {
		return it->second;
	}
	return nullptr;
}


shared_ptr<MapObject> ObjectHash::getObject(const Point& pos) {
	int objID = checkPos(pos);
	if (objID != OBJECT_NOT_FOUND) {
		return getObject(objID);
	}
	return nullptr;
}


std::map<int, shared_ptr<MapObject>> ObjectHash::getObjects(const Point& first, const Point& last) const {
	std::map<int, shared_ptr<MapObject>> retval;
	Point size = last - first;

	for (auto obj : _objects) {
		Point pos = obj.second->getPos();
		auto objInfo = _res->getObjectInfo(obj.second->getType());

		int prio = pos.toRenderPriority(objInfo->mapSize());

		if (pos > first * TILE_MASK && pos < last * TILE_MASK) {
			retval.emplace(prio, obj.second);
		}
	}
	return retval;
}


const std::unordered_map<int, int>* ObjectHash::getObjectMasks() const {
	return &_mask;
}


void ObjectHash::update() {
	for (auto it = _actors.begin(); it != _actors.end(); it++) {
		it->get()->update();
	}
}