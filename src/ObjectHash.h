#pragma once
#include <unordered_map>
#include "Point.h"
#include "Object.h"

class ObjectHash {
	unordered_map<int, shared_ptr<MapObject>> _objects;
	unordered_map<int, int> _mask;
	Point _chunkPos;
	Point _offset;
public:
	ObjectHash();
	~ObjectHash();
	bool setObject();
	bool resetObject();
	bool isFree();
	shared_ptr<MapObject> getObject();
	map<int, shared_ptr<MapObject>> getObjects(const Point& first, const Point& last);
};