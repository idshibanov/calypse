#pragma once
#include "ItemType.h"

class Item {
	int _id;
	static int lastID;
	
	ItemType _type;
	int _quality;
public:
	Item(ItemType type);
	Item(ItemType type, int q);
	Item(const Item& rhs);
	Item& operator=(const Item& rhs);
	~Item();
	int getID() const;
	ItemType getType() const;
	int getQuality() const;
};