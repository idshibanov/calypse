#include "ItemInfo.h"


ItemInfo::ItemInfo(int type, const std::string& name, const Point& size, const Point& offset, 
	               const Point& sprSize, int spriteID, int frame) {
	_type = type;
	_name = name;
	_invSize = size;
	_offset = offset;
	_sprSize = sprSize;
	_spriteID = spriteID;
	_frame = frame;
}

ItemInfo::ItemInfo(const ItemInfo& rhs) {
	_type = rhs._type;
	_name = rhs._name;
	_invSize = rhs._invSize;
	_offset = rhs._offset;
	_sprSize = rhs._sprSize;
	_spriteID = rhs._spriteID;
	_frame = rhs._frame;
}

ItemInfo& ItemInfo::operator= (const ItemInfo& rhs) {
	_type = rhs._type;
	_name = rhs._name;
	_invSize = rhs._invSize;
	_offset = rhs._offset;
	_sprSize = rhs._sprSize;
	_spriteID = rhs._spriteID;
	_frame = rhs._frame;
	return *this;
}

ItemInfo::~ItemInfo() {

}

int ItemInfo::type() const {
	return _type;
}

Point ItemInfo::invSize() const {
	return _invSize;
}

Point ItemInfo::offset() const {
	return _offset;
}

Point ItemInfo::sprSize() const {
	return _sprSize;
}

int ItemInfo::spriteID() const {
	return _spriteID;
}

int ItemInfo::frame() const {
	return _frame;
}

std::string ItemInfo::name() const {
	return _name;
}
