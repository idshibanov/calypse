#include "MapTile.h"
#include "Utils.h"

MapTile::MapTile(char type){
	_type = type;
	_properties = 0;
}

MapTile::MapTile(char type, char sprite){
	_type = type;
	_sprite = sprite;
	_properties = 0;
}

MapTile::~MapTile(){

}

char MapTile::getType() const{
	return _type;
}

void MapTile::setType(char type){
	_type = type;
}

bool MapTile::getSubtile(unsigned pos) {
	if (pos < 26) {
		return getBit(_properties, pos + 1);
	}
	return false;
}

bool MapTile::getSubtile(unsigned x, unsigned y) {
	return getSubtile(y * 5 + x);
}

void MapTile::setSubtile(unsigned pos, bool bit) {
	if (bit) {
		setBit(_properties, pos + 1);
	} else {
		clearBit(_properties, pos + 1);
	}
}