#pragma once
#include <bitset>
#include "Main.h"
#include "MapTile.h"
#include "ObjectHash.h"
#include "ResourceCtl.h"
#include "Item.h"

class Actor;
class MapObject;

#define TD_MAP_ROWS 40
#define TD_MAP_COLS 40

class AStarSearch;
class EventService;

class LocalMap {
	// services
	weak_ptr<EventService> _events;
	shared_ptr<ResourceCtl> _res;
	weak_ptr<AStarSearch> _pFinder;

	// containers
	std::vector<MapTile> _tiles;
	std::multimap<Point, shared_ptr<Item>, cmpPointsStrict> _groundItems;
	ObjectHash _objects;
	shared_ptr<Actor> _actor;

	// state
	unsigned int _rowmax;
	unsigned int _colmax;
	int _xmax;

	// private methods
	std::multimap<Point, shared_ptr<Item>>::iterator findItem(const Point& pos);

	// forbidden to copy
	LocalMap(const LocalMap&) = delete;
	LocalMap& operator=(const LocalMap&) = delete;
public:
	LocalMap(shared_ptr<ResourceCtl> res);
	~LocalMap();
	void generate(weak_ptr<AStarSearch> pf, weak_ptr<EventService> ev);
	short getTileType(const Point& mapPos) const;
	unsigned int getRowMax() const;
	unsigned int getColMax() const;
	shared_ptr<Actor> getPrimaryActor();
	std::multimap<int, shared_ptr<MapObject>> getObjects(const Point& first, const Point& last);
	const std::unordered_map<int, int>* getObjectMasks() const;
	shared_ptr<MapObject> getObject(int);
	bool toggleObject(const Point& coord);
	bool resetObject(const Point& coord);
	bool addObject(shared_ptr<MapObject> obj);
	std::multimap<Point, shared_ptr<Item>> getItems(const Point& first, const Point& last);
	void putItem(const Point& pos, shared_ptr<Item> item);
	shared_ptr<Item> grabItem(const Point& pos);
	bool tileExists(const Point& mapPos) const;
	bool tileIsFree(const Point& mapPos) const;
	bool isFree(const Point& pos) const;
	void setTile(const Point& mapPos, unsigned value);
	void processAction(const Point& mapPos, int id);
	void update();
};