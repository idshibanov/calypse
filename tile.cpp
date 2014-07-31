#include "tile.h"

MapTile::MapTile(short type){
    _type = type;
}

MapTile::MapTile(short type, const shared_ptr<GameObject>& obj){
    _type = type;
    _obj = obj;
}

MapTile::~MapTile(){
    
}

short MapTile::getType() const{
    return _type;
}

weak_ptr<GameObject> MapTile::getObject() const{
    return _obj;
}

void MapTile::setType(short type){
    _type = type;
}

void MapTile::setObject(weak_ptr<GameObject> obj){
    _obj = obj;
}

void MapTile::resetObject(){
    _obj.reset();
}