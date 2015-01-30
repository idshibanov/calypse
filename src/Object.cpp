#include "object.h"

MapObject::MapObject(Point pos, Point size, short type)
	                : _pos(pos), _size(size), _type(type) {
    _deleted = false;
}

MapObject::MapObject(MapObject& rhs){
    *this = rhs;
}

MapObject& MapObject::operator=(MapObject& rhs){
    _pos = rhs._pos;
	_size = rhs._size;
    _type = rhs._type;
    _deleted = rhs._deleted;
    return *this;
}

MapObject::~MapObject(){

}

int MapObject::getXPos(){
	return _pos._x;
}

int MapObject::getYPos(){
    return _pos._y;
}

Point MapObject::getPos() {
	return _pos;
}

Point MapObject::getSize() {
	return _size;
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



Actor::Actor(short type, Point pos, Point size, int defaultSprite, weak_ptr<AStarSearch> pf)
	: MapObject(pos, size, type) {
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
	_pos._x = x;
}

void Actor::setYPos(int y) {
	_pos._y = y;
}

int Actor::getSprite() {
	return _spriteID;
}

void Actor::setDestination(const Point& mod) {
	_timer = 0;
	_destX = mod._x;
	_destY = mod._y;

	cout << "Dest: " << mod._x << "," << mod._y << endl;
	_path = _pathFinder.lock()->searchPath(_pos._x, _pos._y, _destX, _destY);
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
			Point mod(nextNode._mapX, nextNode._mapY);
			mod = (mod - (_pos / TILE_MASK)) * SUBTILE_MASK;
			int dir = getDirection(mod);

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

			_pos += mod;
			if (_pos._x / TILE_MASK == nextNode._mapX && _pos._y / TILE_MASK == nextNode._mapY) {
				_path.erase(_path.begin());
			}
		}
	}
}


int Actor::getDirection(const Point& mod) {
	if (mod._x < 0 && mod._y < 0) {
		return 7;
	} else if (mod._x == 0 && mod._y < 0) {
		return 8;
	} else if (mod._x > 0 && mod._y < 0) {
		return 9;
	} else if (mod._x < 0 && mod._y == 0) {
		return 4;
	} else if (mod._x > 0 && mod._y == 0) {
		return 6;
	} else if (mod._x < 0 && mod._y > 0) {
		return 1;
	} else if (mod._x == 0 && mod._y > 0) {
		return 2;
	} else if (mod._x > 0 && mod._y > 0) {
		return 3;
	}
	return 5;
}