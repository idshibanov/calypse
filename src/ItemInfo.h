#pragma once
#include "ItemType.h"
#include "Point.h"

class ItemInfo {
	ItemType _type;
	Point _offset;
	Point _sprSize;
	int _spriteID;
public:
	ItemInfo(ItemType, const Point&, const Point&, int);
	ItemInfo(const ItemInfo&);
	virtual ~ItemInfo();
	ItemInfo& operator=(const ItemInfo&);
	int type() const;
	Point offset() const;
	Point sprSize() const;
	int spriteID() const;
};