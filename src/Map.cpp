#include <cstdlib>
#include <ctime>
#include "Map.h"
#include "Pathfinder.h"

LocalMap::LocalMap() {

}

LocalMap::~LocalMap(){

}

void LocalMap::generate(weak_ptr<AStarSearch> pf) {
	_pFinder = pf;
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
	int objPos = 0;
	int tempX = 0;
	int tempY = 0;

	for (unsigned row = 0; row < _rowmax; row += 10){
		for (unsigned col = 0; col < _colmax; col += 10){
			for (int k = 0; k < objMaxDensity; k++){
				objPos = rand() % 100;
				tempX = col + (objPos % 10);
				tempY = row + (objPos / 10);
				//shared_ptr<GameObject> tempObj(new GameObject(tempX, tempY, 79));
				//_objects.push_back(tempObj);
				//_tiles[tempY * _colmax + tempX].setObject(tempObj);
			}
		}
	}

	// Player - temporary
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

bool LocalMap::tileExists(unsigned mapID) const {
	return mapID < (_rowmax * _colmax);
}

bool LocalMap::tileExists(unsigned tileX, unsigned tileY) const {
	return tileY < _rowmax && tileX < _colmax;
}

bool LocalMap::tileIsFree(unsigned mapID) const {
	bool retval = false;
	if (tileExists(mapID)) {
		//if(_tiles[mapID].getObject().expired())
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
}