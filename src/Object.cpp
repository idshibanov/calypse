#include "Object.h"
#include "Pathfinder.h"

int MapObject::lastID = 0;

MapObject::MapObject(Point pos, short type)
	                : _pos(pos),  _type(type) {
	_id = lastID++;
	_spriteID = 0;
}

MapObject::MapObject(MapObject& rhs) {
	_pos = rhs._pos;
	_type = rhs._type;
}

MapObject& MapObject::operator=(MapObject& rhs){
    _pos = rhs._pos;
    _type = rhs._type;
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

int MapObject::getType(){
    return _type;
}

int MapObject::getID() {
	return _id;
}

int MapObject::getSprite() {
	return _spriteID;
}

void MapObject::setSprite(int id) {
	_spriteID = id;
}



Actor::Actor(short type, Point pos, int defaultSprite)
	: MapObject(pos, type) {
	_spriteID = defaultSprite;
	_dir = 1;
	_static = true;
	_action = nullptr;
}

Actor::Actor(Actor& rhs) : MapObject(rhs) {
	_spriteID = rhs._spriteID;
	_dir = rhs._dir;
	_static = rhs._static;
	_action = rhs._action;
}

Actor& Actor::operator=(Actor& rhs) {
	MapObject::operator=(rhs);
	_spriteID = rhs._spriteID;
	_dir = rhs._dir;
	_static = rhs._static;
	_action = rhs._action;
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

void Actor::setAction(shared_ptr<Action> act) {
	_action = act;
}

void Actor::update() {
	if (_action) {
		if (_action->isActive()) {
			_action->update();
		} else {
			auto next = _action->getNext();
			if (next) {
				_action = next;
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

void Actor::move(const Point& mod) {
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
}