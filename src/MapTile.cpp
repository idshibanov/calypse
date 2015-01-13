#include "MapTile.h"

MapTile::MapTile(char type){
	_type = type;
}

MapTile::MapTile(char type, char sprite){
	_type = type;
	_sprite = sprite;
}

MapTile::~MapTile(){

}

char MapTile::getType() const{
	return _type;
}

void MapTile::setType(char type){
	_type = type;
}