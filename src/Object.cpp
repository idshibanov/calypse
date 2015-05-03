#include "Object.h"
#include "EventService.h"

int MapObject::lastID = 0;

MapObject::MapObject(short type, const Point& pos)
	                 : _type(type), _pos(pos) {
	_id = lastID++;
	_spriteID = 0;
	_dragged = false;
}

MapObject::MapObject(MapObject& rhs) {
	_pos = rhs._pos;
	_type = rhs._type;
	_dragged = rhs._dragged;
}

MapObject& MapObject::operator=(MapObject& rhs){
    _pos = rhs._pos;
	_type = rhs._type;
	_dragged = rhs._dragged;
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
	if (_dragged && _masterPos) return *_masterPos;
	return _pos;
}

void MapObject::setPos(const Point& pos) {
	_pos = pos;
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

void MapObject::dragged(bool state) {
	_dragged = state;
}

bool MapObject::isDragged() const {
	return _dragged;
}

void MapObject::lift(Point* pos) {
	_masterPos = pos;

	if (pos != nullptr) {
		_dragged = true;
	} else {
		_dragged = false;
	}
}



Actor::Actor(short type, const Point& pos, int defaultSprite)
	: MapObject(type, pos) {
	_state = make_shared<ActorState>();
	_spriteID = defaultSprite;
	_dir = 1;
	_static = true;
	_action = nullptr;
}

Actor::Actor(Actor& rhs) : MapObject(rhs) {
	_state = rhs._state;
	_spriteID = rhs._spriteID;
	_dir = rhs._dir;
	_static = rhs._static;
	_action = rhs._action;
}

Actor& Actor::operator=(Actor& rhs) {
	MapObject::operator=(rhs);
	_state = rhs._state;
	_spriteID = rhs._spriteID;
	_dir = rhs._dir;
	_static = rhs._static;
	_action = rhs._action;
	return *this;
}

Actor::~Actor() {

}

shared_ptr<ActorState> Actor::getState() const {
	return _state;
}

void Actor::setXPos(int x) {
	_pos._x = x;
}

void Actor::setYPos(int y) {
	_pos._y = y;
}

void Actor::setAction(shared_ptr<Action> act) {
	if (_action) {
		_action->chainAction(act);
	} else {
		_action = act;
		_action->start();
	}
}

void Actor::update() {
	if (_action) {
		if (_action->isActive()) {
			_action->update();
		} else {
			auto next = _action->getNext();
			if (next) {
				_action = next;
				_action->start();
			} else {
				_action = nullptr;
			}
		}
	}
}


bool Actor::isWorking() const {
	if (_action && _action->getType() != ACTION_MOVE) {
		return _action->isActive();
	}
	return false;
}


int Actor::getProgress() const {
	if (_action) return _action->getState();

	return -1;
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

void Actor::pickUp(weak_ptr<MapObject> slave) {
	_slave = slave;
	if (!_slave.expired()) {
		_slave.lock()->lift(&_pos);
	}
}

void Actor::drop(const Point& pos) {
	auto slave = _slave.lock();
	if (slave) {
		slave->setPos(pos);
		slave->lift(nullptr);
	}
	_slave = weak_ptr<MapObject>();
}

bool Actor::isHolding() const {
	return !_slave.expired();
}

weak_ptr<MapObject> Actor::getSlave() {
	return _slave;
}



SmallObject::SmallObject(short type, const Point& pos) : MapObject(type, pos) {
	_dragged = false;
}

SmallObject::~SmallObject() {

}

void SmallObject::pickUp(const Actor* act) {
	_dragged = true;
}



SmartActor::SmartActor(short type, const Point& pos, int defaultSprite, 
	                   weak_ptr<EventService> ev)
	                   : Actor(type, pos, defaultSprite) {
	_events = ev;
}

SmartActor::SmartActor(SmartActor& rhs) : Actor(rhs) {
	_events = rhs._events;
}

SmartActor& SmartActor::operator=(SmartActor& rhs) {
	Actor::operator=(rhs);
	_events = rhs._events;
	return *this;
}

SmartActor::~SmartActor() {

}

void SmartActor::update() {
	if (_action) {
		if (_action->isActive()) {
			_action->update();
		} else {
			auto next = _action->getNext();
			if (next) {
				_action = next;
				_action->start();
			} else {
				_action = nullptr;
			}
		}
	} else {
		_events.lock()->spawnAction(_id);
	}
}