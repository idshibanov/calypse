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
		if (_timer % 8 == 0) {
			AStarNode& nextNode = _path.front();
			int modX = (nextNode._mapX - _xpos / TILE_MASK) * SUBTILE_MASK;
			int modY = (nextNode._mapY - _ypos / TILE_MASK) * SUBTILE_MASK;
			int dir = getDirection(modX, modY);

			_spriteID++;
			switch (dir) {
			// 6 dirs, 8 frames
			// T, TR, R-DR, D, DL, L-TL
			// 6, 7,  4-2,  1, 0,  3-5
			case 1:
				if (_spriteID / 8 != 4) {
					_spriteID = 4 * 8;
				}
				break;
			case 2:
				if (_spriteID / 8 != 3) {
					_spriteID = 3 * 8;
				}
				break;
			case 3:
			case 6:
				if (_spriteID / 8 != 2) {
					_spriteID = 2 * 8;
				}
				break;
			case 4:
			case 7:
				if (_spriteID / 8 != 5) {
					_spriteID = 5 * 8;
				}
				break;
			case 8:
				if (_spriteID / 8 != 0) {
					_spriteID = 0 * 8;
				}
				break;
			case 9:
				if (_spriteID / 8 != 1) {
					_spriteID = 1 * 8;
				}
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


int Actor::getDirection(int x, int y) {
	if (x < 0 && y < 0) {
		return 7;
	} else if (x == 0 && y < 0) {
		return 8;
	} else if (x > 0 && y < 0) {
		return 9;
	} else if (x < 0 && y == 0) {
		return 4;
	} else if (x > 0 && y == 0) {
		return 6;
	} else if (x < 0 && y > 0) {
		return 1;
	} else if (x == 0 && y > 0) {
		return 2;
	} else if (x > 0 && y > 0) {
		return 3;
	}
	return 5;
}