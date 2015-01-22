#pragma once
#include <vector>
#include <bitset>
#include "Main.h"
#include "MapTile.h"
#include "Object.h"

#define TILE_MASK 10
#define SUBTILE_STEPS 5

#define TD_MAP_ROWS 40
#define TD_MAP_COLS 40

class AStarSearch;

class LocalMap {
	weak_ptr<AStarSearch> _pFinder;
	vector<MapTile> _tiles;
	shared_ptr<MapObject> _obj;
	unsigned int _rowmax;
	unsigned int _colmax;
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
	shared_ptr<MapObject> getActor();
	bool tileExists(unsigned mapID) const;
	bool tileExists(unsigned tileX, unsigned tileY) const;
	bool tileIsFree(unsigned mapID) const;
	bool tileIsFree(unsigned tileX, unsigned tileY) const;
	void setTile(unsigned id, unsigned value);
	void processAction(int x, int y, int id);
	void update();
};