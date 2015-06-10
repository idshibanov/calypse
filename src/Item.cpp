#include "Item.h"

int Item::lastID = 0;

Item::Item(int type) {
	_id = lastID++;
	_type = type;
	_quality = C_DEFAULT_QUALITY;
}

Item::Item(int type, int q) {
	_id = lastID++;
	_type = type;
	_quality = q;
}

Item::Item(const Item& rhs) {
	_id = lastID++;
	_type = rhs._type;
	_quality = rhs._quality;
}

Item& Item::operator=(const Item& rhs) {
	_type = rhs._type;
	_quality = rhs._quality;
	return *this;
}

Item::~Item() {
	
}

int Item::getID() const {
	return _id;
}

int Item::getType() const {
	return _type;
}

int Item::getQuality() const {
	return _quality;
}

