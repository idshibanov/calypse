#pragma once

class Item {
	int _id;
	static int lastID;
	
	short _type;
	int _quality;
public:
	Item(int type, int q);
	Item(const Item& rhs);
	Item& operator=(const Item& rhs);
	~Item();
	int getID() const;
	int getType() const;
	int getQuality() const;
};