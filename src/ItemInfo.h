#pragma once
#include "ItemType.h"
#include "Point.h"

class ItemInfo {
	ItemType _type;
	Point _sprSize;
	int _spriteID;
public:
	ItemInfo(ItemType, const Point&, int);
	ItemInfo(const ItemInfo&);
	~ItemInfo();
	ItemInfo& operator=(const ItemInfo&);
	int type() const;
	Point sprSize() const;
	int spriteID() const;
};