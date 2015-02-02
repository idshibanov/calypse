#include <cstdlib>
#include <ctime>
#include "Map.h"
#include "Pathfinder.h"
#include "Object.h"

LocalMap::LocalMap() {

}

LocalMap::~LocalMap(){

}

void LocalMap::generate(weak_ptr<AStarSearch> pf) {
	_pFinder = pf;
	_actor = make_shared<Actor>(0, Point(54, 54), 24, _pFinder);
	srand((unsigned)time(NULL));
	_rowmax = TD_MAP_ROWS;
	_colmax = TD_MAP_COLS;
	for (unsigned row = 0; row < _rowmax; row++){
		for (unsigned col = 0; col < _colmax; col++){
			_tiles.push_back(MapTile(8 + (rand() % 6)));
		}
	}

	unsigned pRow = 0, pCol = 0;
	pRow += rand() % 7;
	while (pRow < _rowmax){
		pCol += rand() % 15;
		while (pCol < _colmax){
			_tiles[pRow * _colmax + pCol].setType(47);
			pCol += rand() % 15;
		}
		pRow += rand() % 7;
		pCol = 0;
	}

	// tree generation (sprite 79)
	int objMaxDensity = 10; // 10% max
	int randOffset = 0;
	//_objects.reserve(_rowmax * _colmax / 100);

	for (unsigned row = 0; row < _rowmax; row += 10){
		for (unsigned col = 0; col < _colmax; col += 10){
			for (int k = 0; k < objMaxDensity; k++){
				randOffset = rand() % 100;
				Point objPos((col + (randOffset % 10)) * TILE_MASK, (row + (randOffset / 10) * TILE_MASK));
				_objects.emplace(objPos.toID(_colmax * TILE_MASK), make_shared<MapObject>(objPos, 1));
				//_tiles[tempY * _colmax + tempX].setObject(tempObj);
			}
		}
	}
}

short LocalMap::getTileType(unsigned x, unsigned y) const {
	return getTileType(y*_colmax + x);
}

short LocalMap::getTileType(unsigned id) const {
	return _tiles[id].getType();
}

unsigned int LocalMap::getRowMax() const {
	return _rowmax;
}

unsigned int LocalMap::getColMax() const {
	return _colmax;
}

unsigned int LocalMap::convertIDToX(unsigned mapID) const {
	return mapID % _colmax;
}

unsigned int LocalMap::convertIDToY(unsigned mapID) const {
	return mapID / _colmax;
}

vector<shared_ptr<MapObject>> LocalMap::getObjects(const Point& first, const Point& last) {
	vector<shared_ptr<MapObject>> retval;
	Point size = last - first;
	Point max = last * TILE_MASK;
	retval.reserve(size._x * size._y / 100);

	/*
	for (int y = first._y * TILE_MASK; y < max._y; y++) {
		for (int x = first._x * TILE_MASK; x < max._x; x++) {
			if (x == _actor->getXPos() && y == _actor->getYPos()) {
				retval.push_back(_actor);
			} else {
				auto obj = _objects.find(y * _colmax * TILE_MASK + x);
				if (obj != _objects.end()) {
					retval.push_back(obj->second);
				}
			}
		}
	}
	*/
	for (auto obj : _objects) {
		Point pos = obj.second->getPos();
		if (pos._x > _actor->getXPos() && pos._y > _actor->getYPos()) {
			retval.push_back(_actor);
		}

		if (pos._x > first._x && pos._y > first._y) {
			retval.push_back(obj.second);
		}
	}
	return retval;
}

shared_ptr<Actor> LocalMap::getActor() {
	return _actor;
}

bool LocalMap::tileExists(unsigned mapID) const {
	return mapID < (_rowmax * _colmax);
}

bool LocalMap::tileExists(unsigned tileX, unsigned tileY) const {
	return tileY < _rowmax && tileX < _colmax;
}

bool LocalMap::tileIsFree(unsigned mapID) const {
	bool retval = false;
	if (tileExists(mapID)) {
		if(_tiles[mapID].getType() != 10)
			retval = true;
	}
	return retval;
}

bool LocalMap::tileIsFree(unsigned tileX, unsigned tileY) const {
	bool retval = false;
	if (tileX < _colmax && tileY < _rowmax)
		retval = tileIsFree(tileY * _colmax + tileX);
	return retval;
}

void LocalMap::setTile(unsigned id, unsigned value) {
	_tiles[id].setType(value);
}

void LocalMap::processAction(int x, int y, int id) {

}

void LocalMap::update() {
	_actor->update();
}