#include "Inventory.h"

Inventory::Inventory() {

}

Inventory::~Inventory() {
	
}


std::vector<shared_ptr<Item>>::iterator Inventory::findItem(ItemType t) {
	for (auto it = _items.begin(); it != _items.end(); it++) {
		if ((*it)->getType() == t) {
			return it;
		}
	}
	return _items.end();
}


std::vector<shared_ptr<Item>> Inventory::findAllItems(ItemType t) const {
	std::vector<shared_ptr<Item>> retval;

	for (auto it : _items) {
		if (it->getType() == t) {
			retval.push_back(it);
		}
	}
	return retval;
}

int Inventory::getItemCount(ItemType t) const {
	return findAllItems(t).size();
}

bool Inventory::useItem(ItemType t) {
	auto item = findItem(t);
	if (item != _items.end()) {
		_items.erase(item);
		return true;
	}
	return false;
}

void Inventory::addItem(shared_ptr<Item> item) {
	_items.push_back(item);
}
