#include <cstdlib>
#include <ctime>
#include "Map.h"
#include "Pathfinder.h"
#include "Object.h"

LocalMap::LocalMap() : _objects(TD_MAP_COLS) {

}

LocalMap::~LocalMap(){

}

void LocalMap::generate(weak_ptr<AStarSearch> pf) {
	_pFinder = pf;
	_actor = make_shared<Actor>(0, Point(54, 54), 24);
	srand((unsigned)time(NULL));
	_rowmax = TD_MAP_ROWS;
	_colmax = TD_MAP_COLS;
	_xmax = _colmax * TILE_MASK;
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

	_objects.setObject(Point(150, 150), Point(20, 30), make_shared<MapObject>(Point(150, 150), 2));
	for (unsigned row = 0; row < _rowmax; row += 10){
		for (unsigned col = 0; col < _colmax; col += 10){
			for (int k = 0; k < objMaxDensity; k++){
				randOffset = rand() % 100;
				Point objPos((col + (randOffset % 10)) * TILE_MASK, (row + (randOffset / 10)) * TILE_MASK);
				cout << "OBJ: " << col + (randOffset % 10) << "," << row + (randOffset / 10);
				cout << "  >> " << objPos._x << "," << objPos._y << " Prio: " << objPos.toRenderPriority() << endl;
				_objects.setObject(objPos, Point(10, 10), make_shared<MapObject>(objPos, 1));
				//_objects.emplace(objPos.toID(_xmax), make_shared<MapObject>(objPos, 1));
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

map<int, shared_ptr<MapObject>> LocalMap::getObjects(const Point& first, const Point& last) {
	//vector<shared_ptr<MapObject>> retval;
	//map<int, shared_ptr<MapObject>> retval;
	//Point size = last - first;

	/*
	retval.reserve(size._x * size._y / 100);
	Point max = last * TILE_MASK;
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

	for (auto obj : _objects) {
		Point pos = obj.second->getPos();

		if (pos > first * TILE_MASK && pos < last * TILE_MASK) {
			retval.insert(obj);
			//retval.push_back(obj.second);
		}
	}
	*/
	auto retval = _objects.getObjects(first, last);
	pair<int, shared_ptr<MapObject>> act(_actor->getPos().toRenderPriority(), _actor);
	retval.insert(act);
	return retval;
}

const unordered_map<int, int>* LocalMap::getObjectMasks() const {
	return _objects.getObjectMasks();
}

shared_ptr<Actor> LocalMap::getActor() {
	return _actor;
}

bool LocalMap::resetObject(const Point& coord) {
	return _objects.resetObject(coord);
}

bool LocalMap::tileExists(unsigned mapID) const {
	return mapID < (_rowmax * _colmax);
}

bool LocalMap::tileExists(unsigned tileX, unsigned tileY) const {
	return tileY < _rowmax && tileX < _colmax;
}

bool LocalMap::tileIsFree(unsigned mapID) const {
	if (tileExists(mapID)) {
		//int pos = (mapID / _colmax * TILE_MASK) * _xmax + (mapID % _colmax) * TILE_MASK;
		Point pos((mapID % _colmax) * TILE_MASK, mapID / _colmax * TILE_MASK);
		if (_objects.isFree(pos)) {
			return true;
		}
	}
	return false;
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