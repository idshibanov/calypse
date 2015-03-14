#include "ObjectInfo.h"


ObjectInfo::ObjectInfo(int type, int sizeX, int sizeY, int offsetX, 
	                   int offsetY, int spriteID, bool drag) {
	_spriteID = spriteID;
	_type = type;
	_size = Point(sizeX, sizeY);
	_offset = Point(offsetX, offsetY);
	_draggable = drag;
}

ObjectInfo::ObjectInfo(ObjectInfo& rhs) {
	_type = rhs._type;
	_size = rhs._size;
	_offset = rhs._offset;
	_spriteID = rhs._spriteID;
	_draggable = rhs._draggable;
}

ObjectInfo& ObjectInfo::operator= (ObjectInfo& rhs) {
	_type = rhs._type;
	_size = rhs._size;
	_offset = rhs._offset;
	_spriteID = rhs._spriteID;
	_draggable = rhs._draggable;
	return *this;
}

ObjectInfo::~ObjectInfo() {

}