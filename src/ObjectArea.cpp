#include "ObjectArea.h"

ObjectArea::ObjectArea(const Point& pos, const Point& size, shared_ptr<MapObject> obj,
	weak_ptr<Sprite> spr, shared_ptr<ObjectInfo> info) : ScreenArea(pos, size) {
	_type = AREA_TYPE_OBJECT;
	_obj = obj;
	_sprite = spr;
	_info = info;
}

ObjectArea::~ObjectArea() {

}

shared_ptr<SpriteSheet> ObjectArea::castSprite() const {
	return std::dynamic_pointer_cast<SpriteSheet>(_sprite.lock());
}

const shared_ptr<ObjectActionArea> ObjectArea::getSubArea(const Point& pos) const {
	return _info->getSubArea(pos - _pos);
}