#include "ObjectArea.h"

ObjectArea::ObjectArea(const Point& pos, const Point& size, shared_ptr<MapObject> obj, SpriteSheet* spr) 
	                   : ScreenArea(pos, size) {
	_type = AREA_TYPE_OBJECT;
	_obj = obj;
	_sprite = spr;
}

ObjectArea::~ObjectArea() {

}
