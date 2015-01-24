#include "object.h"

MapObject::MapObject(int xpos, int ypos, short type) 
                     : _xpos(xpos), _ypos(ypos), _type(type) {
    _deleted = false;
}

MapObject::MapObject(MapObject& rhs){
    *this = rhs;
}

MapObject& MapObject::operator=(MapObject& rhs){
    _xpos = rhs._xpos;
    _ypos = rhs._ypos;
    _type = rhs._type;
    _deleted = rhs._deleted;
    return *this;
}

MapObject::~MapObject(){

}

int MapObject::getXPos(){
    return _xpos;
}

int MapObject::getYPos(){
    return _ypos;
}

int MapObject::getType(){
    return _type;
}

bool MapObject::isDeleted(){
    return _deleted;
}

void MapObject::deleteObject(){
    _deleted = true;
}



Actor::Actor(int xpos, int ypos, short type, int defaultSprite, weak_ptr<AStarSearch> pf)
	: MapObject(xpos, ypos, type) {
	_spriteID = defaultSprite;
	_dir = 1;
	_static = true;
	_pathFinder = pf;
	_destX = 0;
	_destY = 0;
}

Actor::Actor(Actor& rhs) : MapObject(rhs) {
}

Actor& Actor::operator=(Actor& rhs) {
	MapObject::operator=(rhs);
	_spriteID = rhs._spriteID;
	_dir = rhs._dir;
	_static = rhs._static;
	_destX = rhs._destX;
	_destY = rhs._destY;
	_pathFinder = rhs._pathFinder;
	return *this;
}

Actor::~Actor() {

}

void Actor::setXPos(int x) {
	_xpos = x;
}

void Actor::setYPos(int y) {
	_ypos = y;
}

int Actor::getSprite() {
	return _spriteID;
}

void Actor::setDestination(int x, int y) {
	_timer = 0;
	_destX = x;
	_destY = y;
	_path = _pathFinder.lock()->searchPath(_xpos, _ypos, _destX, _destY);
	for (auto it = _path.begin(); it != _path.end(); ++it) {
		std::cout << (*it) << endl;
	}
	std::cout << endl;
}

void Actor::update() {
	if (!_path.empty()) {
		_timer++;
		if (_timer % 25 == 0) {
			AStarNode& nextNode = _path.front();
			_xpos = nextNode._mapX;
			_ypos = nextNode._mapY;
			_path.erase(_path.begin());
		}
	}
}