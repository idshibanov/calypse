#include "ObjectInfo.h"


ObjectInfo::ObjectInfo(int type, const Point& mapSize, const Point& sprSize, const Point& offset, 
	                   int spriteID, int frames, bool lift) {
	_type = type;
	_mapSize = mapSize;
	_sprSize = sprSize;
	_offset = offset;
	_spriteID = spriteID;
	_frames = frames;
	_liftable = lift;
}

ObjectInfo::ObjectInfo(ObjectInfo& rhs) {
	_type = rhs._type;
	_mapSize = rhs._mapSize;
	_sprSize = rhs._sprSize;
	_offset = rhs._offset;
	_spriteID = rhs._spriteID;
	_frames = rhs._frames;
	_liftable = rhs._liftable;
}

ObjectInfo& ObjectInfo::operator= (ObjectInfo& rhs) {
	_type = rhs._type;
	_mapSize = rhs._mapSize;
	_sprSize = rhs._sprSize;
	_offset = rhs._offset;
	_spriteID = rhs._spriteID;
	_frames = rhs._frames;
	_liftable = rhs._liftable;
	return *this;
}

ObjectInfo::~ObjectInfo() {

}

int ObjectInfo::type() const {
	return _type;
}

Point ObjectInfo::mapSize() const {
	return _mapSize;
}

Point ObjectInfo::sprSize() const {
	return _sprSize;
}

Point ObjectInfo::offset() const {
	return _offset;
}

int ObjectInfo::spriteID() const {
	return _spriteID;
}

int ObjectInfo::frames() const {
	return _frames;
}

bool ObjectInfo::liftable() const {
	return _liftable;
}
