#include "ObjectArea.h"

ObjectArea::ObjectArea(const Point& pos, const Point& size, shared_ptr<MapObject> obj, SpriteSheet* spr) 
                       : ScreenArea(pos, size) {
	_obj = obj;
	_sprite = spr;
}

ObjectArea::~ObjectArea() {

}
