#include <cstdlib>
#include <ctime>
#include "Map.h"
#include "EventService.h"
#include "Pathfinder.h"
#include "Object.h"

LocalMap::LocalMap(shared_ptr<ResourceCtl> res) : _objects(res, TD_MAP_COLS * TILE_MASK) {
	_res = res;
}

LocalMap::~LocalMap(){

}

void LocalMap::generate(weak_ptr<AStarSearch> pf, weak_ptr<EventService> ev) {
	_events = ev;
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

	_objects.setObject(make_shared<MapObject>(2, Point(150, 150)));
	_objects.setObject(make_shared<MapObject>(3, Point(80, 90)));
	_objects.setObject(make_shared<SmartActor>(0, Point(110, 110), 24, _events));
	for (unsigned row = 0; row < _rowmax; row += 10){
		for (unsigned col = 0; col < _colmax; col += 10){
			for (int k = 0; k < objMaxDensity; k++){
				randOffset = rand() % 100;
				Point objPos((col + (randOffset % 10)) * TILE_MASK, (row + (randOffset / 10)) * TILE_MASK);
				cout << "OBJ: " << col + (randOffset % 10) << "," << row + (randOffset / 10);
				cout << "  >> " << objPos._x << "," << objPos._y << " Prio: " << objPos.toRenderPriority() << endl;
				_objects.setObject(make_shared<MapObject>(1, objPos));
			}
		}
	}

	_groundItems.emplace(Point(40, 40), make_shared<Item>(C_ITEM_BRANCH));
	_groundItems.emplace(Point(48, 48), make_shared<Item>(C_ITEM_BRANCH));
}

short LocalMap::getTileType(const Point& mapPos) const {
	return _tiles[mapPos.toID(_colmax)].getType();
}

unsigned int LocalMap::getRowMax() const {
	return _rowmax;
}

unsigned int LocalMap::getColMax() const {
	return _colmax;
}


std::multimap<int, shared_ptr<MapObject>> LocalMap::getObjects(const Point& first, const Point& last) {
	auto retval = _objects.getObjects(first, last);
	std::pair<int, shared_ptr<MapObject>> act(_actor->getPos().toRenderPriority()-1, _actor);
	retval.insert(act);
	return retval;
}

const std::unordered_map<int, int>* LocalMap::getObjectMasks() const {
	return _objects.getObjectMasks();
}

shared_ptr<MapObject> LocalMap::getObject(int id) {
	return _objects.getObject(id);
}

shared_ptr<Actor> LocalMap::getPrimaryActor() {
	return _actor;
}

bool LocalMap::toggleObject(const Point& coord) {
	return _objects.toggleObject(coord);
}

bool LocalMap::resetObject(const Point& coord) {
	return _objects.resetObject(coord);
}

bool LocalMap::addObject(shared_ptr<MapObject> obj) {
	return _objects.setObject(obj);
}


std::multimap<Point, shared_ptr<Item>>::iterator LocalMap::findItem(const Point& pos) {
	return _groundItems.find(pos);
}

std::multimap<Point, shared_ptr<Item>> LocalMap::getItems(const Point& first, const Point& last) {
	std::multimap<Point, shared_ptr<Item>> retval;
	Point topLeft = first.mul(TILE_MASK);
	Point botRight = last.mul(TILE_MASK);

	for (auto item : _groundItems) {
		if (item.first > botRight) {
			break;
		} else if (!(item.first < topLeft)) {
			retval.insert(item);
		}
	}
	return retval;
}

void LocalMap::putItem(const Point& pos, shared_ptr<Item> item) {
	_groundItems.emplace(pos, item);
}

shared_ptr<Item> LocalMap::grabItem(const Point& pos) {
	shared_ptr<Item> retval;
	auto it = findItem(pos);
	if (it != _groundItems.end()) {
		retval = it->second;
		_groundItems.erase(it);
		return retval;
	}
	return retval;
}


bool LocalMap::tileExists(const Point& mapPos) const {
	return mapPos._x < _rowmax && mapPos._y < _colmax;
}

bool LocalMap::tileIsFree(const Point& mapPos) const {
	if (tileExists(mapPos)) {
		return _objects.isFree(mapPos);
	}
	return false;
}

bool LocalMap::isFree(const Point& pos) const {
	if (pos._x >= 0 && pos._x < _xmax &&
		pos._y >= 0 && pos._y < _rowmax*TILE_MASK) {
		return _objects.isFree(pos);
	}
	return false;
}

void LocalMap::setTile(const Point& mapPos, unsigned value) {
	_tiles[mapPos.toID(_colmax)].setType(value);
}

void LocalMap::processAction(const Point& mapPos, int id) {

}

void LocalMap::update() {
	_actor->update();
	_objects.update();
}