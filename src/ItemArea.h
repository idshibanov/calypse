#pragma once
#include "ScreenArea.h"
#include "Item.h"

class ItemArea : public ScreenArea {
	weak_ptr<SpriteSheet> _sprite;
public:
	shared_ptr<Item> _item;
	ItemArea(const Point&, const Point&, shared_ptr<Item>, weak_ptr<SpriteSheet>);
	~ItemArea();
	shared_ptr<SpriteSheet> getSprite() const;
};
