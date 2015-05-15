#pragma once
#include <vector>

#include "Main.h"
#include "Item.h"

class Inventory {
	std::vector<shared_ptr<Item>> _items;

	std::vector<shared_ptr<Item>>::iterator findItem(ItemType t);
	std::vector<shared_ptr<Item>> findAllItems(ItemType t) const;
public:
	Inventory();
	~Inventory();
	int getItemCount(ItemType t) const;
	bool useItem(ItemType t);
	void addItem(shared_ptr<Item> item);
};