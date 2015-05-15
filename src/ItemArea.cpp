#include "ItemArea.h"

ItemArea::ItemArea(const Point& pos, const Point& size, shared_ptr<Item> item, 
	               weak_ptr<SpriteSheet> spr) : ScreenArea(pos, size) {
	_type = AREA_TYPE_ITEM;
	_item = item;
	_sprite = spr;
}

ItemArea::~ItemArea() {

}

shared_ptr<SpriteSheet> ItemArea::getSprite() const {
	if (!_sprite.expired()) {
		return _sprite.lock();
	}
	return nullptr;
}