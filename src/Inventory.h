#pragma once
#include <map>
#include <set>
#include <vector>

#include "Main.h"
#include "Item.h"
#include "Point.h"

class Inventory {
	std::vector<shared_ptr<Item>> _items;

	std::vector<shared_ptr<Item>>::iterator findItem(ItemType t);
	std::vector<shared_ptr<Item>>::iterator findItem(int id);
	std::vector<shared_ptr<Item>> findAllItems(ItemType t) const;

	std::vector<int> _itemMasks; // init with -1
	Rect _invArea; // for vector iterations

	bool itemFits(const Point& cell, const Point& size); // check if item will fit

	int countItems(ItemType t);
	std::set<int> getItemsOnArea(const Point& cell, const Point& size); // check what's going on in the area

	int getItem(const Point& cell); // def. private
	void setCell(int id, const Point& cell);
	void clearCell(const Point& cell); // def. private
	void setItem(int id, const Point& cell, const Point& size); // def. private search masks and replace all ids with -1
	void clearItem(int id); // def. private search masks and replace all ids with -1

	// push item to inventory (find first available spot)
	// useItem for craft (don't care which one)
	// takeItem for mouse clicks
public:
	Inventory();
	~Inventory();
	Point getSize() const;
	bool resize(Point diff);
	std::map<Point, shared_ptr<Item>, cmpPointsStrict> getItemList();

	int getItemCount(ItemType t) const;
	bool useItem(ItemType t);
	void addItem(shared_ptr<Item> item);

	bool isFree(const Point& cell);
	shared_ptr<Item> findItemByID(int id);
	bool putItem(shared_ptr<Item> item, const Point& cell); // iterate, check Fits, update masks
	shared_ptr<Item> takeItem(int id);
	bool useItems(ItemType t, int count); // search & remove, you better to do something with result
	shared_ptr<Item> forceItem(shared_ptr<Item> item, const Point& cell); // counts items, if 0 -> return same id, if 1 -> return old, if 2+ -> -1

	void debug() const;
};