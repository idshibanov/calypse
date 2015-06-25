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
	shared_ptr<ResourceCtl> _res;
	std::unordered_map<int, shared_ptr<MapObject>> _objects;
	std::unordered_map<int, int> _mask;
	std::vector<shared_ptr<Actor>> _actors;
	Point _chunkPos;
	Point _offset;
	int _size;
	ObjectHash(const ObjectHash&);
	ObjectHash& operator= (const ObjectHash&);
public:
	ObjectHash(shared_ptr<ResourceCtl>, int size);
	~ObjectHash();
	void clear();
	bool setObject(shared_ptr<MapObject> obj);
	bool setObject(const Point& pos, shared_ptr<MapObject> obj);
	bool toggleObject(const Point& pos);
	bool resetObject(const Point& pos);
	bool isFree(const Point& pos) const;
	int checkPos(const Point& pos) const;
	std::vector<Point> searchForObject(const Point&);
	shared_ptr<MapObject> getObject(int);
	shared_ptr<MapObject> getObject(const Point&);
	int getObjectID(const Point&);
	std::multimap<int, shared_ptr<MapObject>> getObjects(const Point& first, const Point& last) const;
	const std::unordered_map<int, int>* getObjectMasks() const;
	void update();
};