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

	cout << "Dest: " << x << "," << y << endl;
	_path = _pathFinder.lock()->searchPath(_xpos, _ypos, _destX, _destY);
	for (auto it = _path.begin(); it != _path.end(); ++it) {
		std::cout << (*it) << endl;
	}
	std::cout << endl;
}

void Actor::update() {
	if (!_path.empty()) {
		_timer++;
		if (_timer % 6 == 0) {
			AStarNode& nextNode = _path.front();
			int modX = (nextNode._mapX - _xpos / TILE_MASK) * SUBTILE_MASK;
			int modY = (nextNode._mapY - _ypos / TILE_MASK) * SUBTILE_MASK;

			switch (getDirection(modX, modY)) {
			// 6 dirs, 8 frames, T-TR, R , RD, D-LD, L, TL
			case 0:
			case 1:
				_spriteID = 3 * 8;
				break;
			case 2:
				_spriteID = 2 * 8;
				break;
			case 3:
				_spriteID = 4 * 8;
				break;
			case 4:
				_spriteID = 1 * 8;
				break;
			case 5:
				_spriteID = 5 * 8;
				break;
			case 6:
			case 7:
				_spriteID = 0;
				break;
			}

			_xpos += modX;
			_ypos += modY;
			if (_xpos / TILE_MASK == nextNode._mapX && _ypos / TILE_MASK == nextNode._mapY) {
				_path.erase(_path.begin());
			}
		}
	}
}

// 7 8 9      6 7 8     -1, 1
// 4   6   => 3 4 5  => -1, 0
// 1 2 3      0 1 2     -1,-1

int Actor::getDirection(int x, int y) {
	if (x < 0 && y < 0) {
		return 0;
	} else if (x < 0 && y == 0) {
		return 1;
	} else if (x < 0 && y > 0) {
		return 2;
	} else if (x == 0 && y < 0) {
		return 3;
	} else if (x == 0 && y > 0) {
		return 4;
	} else if (x > 0 && y < 0) {
		return 5;
	} else if (x > 0 && y == 0) {
		return 6;
	} else if (x > 0 && y > 0) {
		return 7;
	}
	return -1;
}