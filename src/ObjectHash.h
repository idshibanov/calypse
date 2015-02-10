#pragma once
#include <unordered_map>
#include "Point.h"
#include "Object.h"

class ObjectHash {
	unordered_map<int, shared_ptr<MapObject>> _objects;
	unordered_map<int, int> _mask;
	Point _chunkPos;
	Point _offset;
	int _size;
public:
	ObjectHash();
	~ObjectHash();
	bool setObject(const Point& pos, const Point& size, shared_ptr<MapObject> obj);
	bool resetObject(const Point& pos);
	bool isFree(const Point& pos);
	shared_ptr<MapObject> getObject(const Point&);
	int getObjectID(const Point&);
	map<int, shared_ptr<MapObject>> getObjects(const Point& first, const Point& last);
};