#include "ObjectInfo.h"


ObjectInfo::ObjectInfo(int type, int sizeX, int sizeY, int offsetX, int offsetY, int spriteID) {
	_spriteID = spriteID;
	_type = type;
	_size = Point(sizeX, sizeY);
	_offset = Point(offsetX, offsetY);
}

ObjectInfo::ObjectInfo(ObjectInfo& rhs) {
	_type = rhs._type;
	_size = rhs._size;
	_offset = rhs._offset;
}

ObjectInfo& ObjectInfo::operator= (ObjectInfo& rhs) {
	_type = rhs._type;
	_size = rhs._size;
	_offset = rhs._offset;
	return *this;
}

ObjectInfo::~ObjectInfo() {

}