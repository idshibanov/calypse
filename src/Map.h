#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <bitset>
#include "Main.h"
#include "MapTile.h"
#include "ObjectHash.h"
#include "ResourceCtl.h"

class Actor;
class MapObject;

#define TD_MAP_ROWS 40
#define TD_MAP_COLS 40

class AStarSearch;

class LocalMap {
	weak_ptr<AStarSearch> _pFinder;
	vector<MapTile> _tiles;
	//unordered_map<int, shared_ptr<MapObject>> _objects;
	ObjectHash _objects;
	shared_ptr<Actor> _actor;
	unsigned int _rowmax;
	unsigned int _colmax;
	int _xmax;
public:
	LocalMap();
	~LocalMap();
	void generate(weak_ptr<AStarSearch> pf);
	short getTileType(unsigned x, unsigned y) const;
	short getTileType(unsigned id) const;
	unsigned int getRowMax() const;
	unsigned int getColMax() const;
	unsigned int convertIDToX(unsigned mapID) const;
	unsigned int convertIDToY(unsigned mapID) const;
	shared_ptr<Actor> getActor();
	map<int, shared_ptr<MapObject>> getObjects(const Point& first, const Point& last);
	const unordered_map<int, int>* getObjectMasks() const;
	bool resetObject(const Point& coord);
	bool tileExists(unsigned mapID) const;
	bool tileExists(unsigned tileX, unsigned tileY) const;
	bool tileIsFree(unsigned mapID) const;
	bool tileIsFree(unsigned tileX, unsigned tileY) const;
	void setTile(unsigned id, unsigned value);
	void processAction(int x, int y, int id);
	void update();
};