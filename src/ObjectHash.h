#pragma once
#include <vector>
#include <map>
#include <unordered_map>

#include "Point.h"
#include "Object.h"

#define OBJECT_NOT_FOUND -1

// TODO: use offset & pos to break hash into chunks

class ObjectHash {
	unordered_map<int, shared_ptr<MapObject>> _objects;
	unordered_map<int, int> _mask;
	Point _chunkPos;
	Point _offset;
	int _size;
public:
	ObjectHash(int size);
	~ObjectHash();
	bool setObject(const Point& pos, const Point& size, shared_ptr<MapObject> obj);
	bool resetObject(const Point& pos);
	bool isFree(const Point& pos) const;
	int checkPos(const Point& pos) const;
	vector<Point> searchForObject(const Point&);
	shared_ptr<MapObject> getObject(int);
	shared_ptr<MapObject> getObject(const Point&);
	int getObjectID(const Point&);
	map<int, shared_ptr<MapObject>> getObjects(const Point& first, const Point& last) const;
};