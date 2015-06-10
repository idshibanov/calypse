#pragma once
#include "ItemType.h"

class Item {
	int _id;
	static int lastID;
	
	int _type;
	int _quality;
public:
	Item(int type);
	Item(int type, int q);
	Item(const Item& rhs);
	Item& operator=(const Item& rhs);
	virtual ~Item();
	int getID() const;
	int getType() const;
	int getQuality() const;
};