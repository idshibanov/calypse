#pragma once
#include <vector>
#include <map>
#include <unordered_map>

#include "Point.h"
#include "Object.h"
#include "ResourceCtl.h"

#define OBJECT_NOT_FOUND -1

// TODO: use offset & pos to break hash into chunks

class ObjectHash {
	std::unordered_map<int, std::shared_ptr<MapObject>> _objects;
	std::unordered_map<int, int> _mask;
	Point _chunkPos;
	Point _offset;
	int _size;
public:
	ObjectHash(int size);
	~ObjectHash();
	bool setObject(const Point& pos, const Point& size, std::shared_ptr<MapObject> obj);
	bool setObject(const Point& pos, std::shared_ptr<MapObject> obj);
	bool toggleObject(const Point& pos);
	bool resetObject(const Point& pos);
	bool isFree(const Point& pos) const;
	int checkPos(const Point& pos) const;
	std::vector<Point> searchForObject(const Point&);
	std::shared_ptr<MapObject> getObject(int);
	std::shared_ptr<MapObject> getObject(const Point&);
	int getObjectID(const Point&);
	std::map<int, std::shared_ptr<MapObject>> getObjects(const Point& first, const Point& last) const;
	const std::unordered_map<int, int>* getObjectMasks() const;
};