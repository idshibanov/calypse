#include "ItemInfo.h"


ItemInfo::ItemInfo(ItemType type, const Point& sprSize, int spriteID) {
	_type = type;
	_sprSize = sprSize;
	_spriteID = spriteID;
}

ItemInfo::ItemInfo(const ItemInfo& rhs) {
	_type = rhs._type;
	_sprSize = rhs._sprSize;
	_spriteID = rhs._spriteID;
}

ItemInfo& ItemInfo::operator= (const ItemInfo& rhs) {
	_type = rhs._type;
	_sprSize = rhs._sprSize;
	_spriteID = rhs._spriteID;
	return *this;
}

ItemInfo::~ItemInfo() {

}

int ItemInfo::type() const {
	return _type;
}

Point ItemInfo::sprSize() const {
	return _sprSize;
}

int ItemInfo::spriteID() const {
	return _spriteID;
}