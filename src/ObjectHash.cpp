#include <functional>
#include "ObjectHash.h"

ObjectHash::ObjectHash(int size) {
	_size = size;
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

bool ObjectHash::resetObject(const Point& pos) {
	auto obj = getObject(pos);
	if (obj != nullptr) {
		Rect area(obj->getPos(), Point(1,1));

		std::function<void(const Point&)> resetter = [this](const Point& pos) {
			_mask.erase(pos.toID(_size));
		};

		area.iterate(resetter);
		return true;
	}
	return false;
}

bool ObjectHash::isFree(const Point& pos) const {
	return checkPos(pos) == OBJECT_NOT_FOUND;
}

int ObjectHash::checkPos(const Point& pos) const {
	auto it = _mask.find(pos.toID(_size));
	if(it != _mask.end()) {
		return it->second;
	}
	return OBJECT_NOT_FOUND;
}

vector<Point> ObjectHash::searchForObject(const Point& pos) {
	vector<Point> retval;
	
	int objID = checkPos(pos);
	if(objID != OBJECT_NOT_FOUND) {
		retval.push_back(pos);
		
	}	
	/*
	private recursive function (pass pos + id), concat return results
	AB.reserve( A.size() + B.size() ); // preallocate memory
	AB.insert( AB.end(), A.begin(), A.end() );
	AB.insert( AB.end(), B.begin(), B.end() );
	*/
	return retval;
}


shared_ptr<MapObject> ObjectHash::getObject(int objID) {
	auto it = _objects.find(objID);
	if(it != _objects.end()) {
		return it->second;
	}
	return nullptr;
}

shared_ptr<MapObject> ObjectHash::getObject(const Point& pos) {
	int objID = checkPos(pos);
	if(objID != OBJECT_NOT_FOUND) {
		return getObject(objID);
	}
	return nullptr;
}

map<int, shared_ptr<MapObject>> ObjectHash::getObjects(const Point& first, const Point& last) const {
	map<int, shared_ptr<MapObject>> retval;
	Point size = last - first;

	for (auto obj : _objects) {
		Point pos = obj.second->getPos();
		int prio = pos.toRenderPriority();

		// pass ResourceCtl and get proper size
		if (obj.second->getType() == 1) {
			prio = pos.toRenderPriority(Point(10,10));
		}

		if (pos > first * TILE_MASK && pos < last * TILE_MASK) {
			retval.emplace(prio, obj.second);
		}
	}
	return retval;
}