#include <iomanip>
#include "Inventory.h"

Inventory::Inventory() : _invArea(Point(), Point(5, 5)), _itemMasks(25, -1) {

}

Inventory::~Inventory() {
	
}


Point Inventory::getSize() const {
	return _invArea._size;
}


std::map<Point, shared_ptr<Item>, cmpPointsStrict> Inventory::getItemList() {
	std::map<Point, shared_ptr<Item>, cmpPointsStrict> retval;
	std::set<int> found;

	std::function<void(const Point&)> counter = [this, &found, &retval](const Point& pos) {
		int id = getItem(pos);
		if (id != -1 && found.insert(id).second) {
			if (!retval.emplace(pos, findItemByID(id)).second) {
				cout << "FAIL";
				//retval.emplace(pos, findItemByID(id));
			}
		}
	};

	_invArea.iterate(counter, 1);

	return retval;
}


shared_ptr<Item> Inventory::findItemByID(int id) {
	for (auto it : _items) {
		if (it->getID() == id) {
			return it;
		}
	}
	return nullptr;
}


std::vector<shared_ptr<Item>>::iterator Inventory::findItem(ItemType t) {
	for (auto it = _items.begin(); it != _items.end(); it++) {
		if ((*it)->getType() == t) {
			return it;
		}
	}
	return _items.end();
}

std::vector<shared_ptr<Item>>::iterator Inventory::findItem(int id) {
	for (auto it = _items.begin(); it != _items.end(); it++) {
		if ((*it)->getID() == id) {
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




bool Inventory::itemFits(const Point& cell, const Point& size) {
	Rect itemArea(cell, size);

	std::function<bool(const Point&)> search = [this](const Point& pos) {
		return isFree(pos);
	};

	return itemArea.iterate(search, true, 1);
}

bool Inventory::putItem(shared_ptr<Item> item, const Point& cell) {
	Point size(1, 1);

	if (itemFits(cell, size)) {
		addItem(item);
		setItem(item->getID(), cell, size);
		return true;
	}
	return false;
}

shared_ptr<Item> Inventory::takeItem(int id) {
	shared_ptr<Item> retval = nullptr;
	auto it = findItem(id);

	if (it != _items.end()) {
		retval = (*it);
		_items.erase(it);
		clearItem(id);
	}
	return retval;
}

void Inventory::clearItem(int id) {
	std::function<void(const Point&)> cleaner = [this, &id](const Point& pos) {
		if (getItem(pos) == id) {
			clearCell(pos);
		}
	};

	_invArea.iterate(cleaner, 1);
}

void Inventory::setItem(int id, const Point& cell, const Point& size) {
	Rect itemArea(cell, size);

	std::function<void(const Point&)> setter = [this, &id](const Point& pos) {
		setCell(id, pos);
	};

	return itemArea.iterate(setter, 1);
}

bool Inventory::useItems(ItemType t, int count) {
	return false;
}


bool Inventory::isFree(const Point& cell) {
	return getItem(cell) == -1;
}


int Inventory::getItem(const Point& cell) {
	if (cell > _invArea._size) {
		return -1;
	}
	return _itemMasks[cell.toID(_invArea._size._x)];
}


void Inventory::setCell(int id, const Point& cell) {
	_itemMasks[cell.toID(_invArea._size._x)] = id;
}

void Inventory::clearCell(const Point& cell) {
	setCell(-1, cell);
}

int Inventory::countItems(ItemType t) {
	int retval = 0;

	std::function<void(const Point&)> counter = [this, &retval, &t](const Point& pos) {
		auto item = findItemByID(getItem(pos));

		if (item && item->getType() == t) {
			retval++;
		}
	};

	_invArea.iterate(counter, 1);
	return retval;
}

std::set<int> Inventory::getItemsOnArea(const Point& cell, const Point& size) {
	Rect itemArea(cell, size);
	std::set<int> found;

	std::function<void(const Point&)> counter = [this, &found](const Point& pos) {
		if (!isFree(pos)) {
			found.insert(getItem(pos));
		}
	};

	itemArea.iterate(counter, 1);
	return found;
}

shared_ptr<Item> Inventory::forceItem(shared_ptr<Item> item, const Point& cell) {
	Point size(1, 1);
	auto found = getItemsOnArea(cell, size);

	if (found.size() == 0 ) {
		putItem(item, cell);
	} else if (found.size() == 1) {
		int oldID = *found.begin();

		auto ret = takeItem(oldID);
		putItem(item, cell);

		return ret;
	}
	return nullptr;
}

void Inventory::debug() const {
	for (int i = 0; i < _invArea._size._y; i++) {
		cout << "==========================" << endl;
		cout << "| ";
		for (int j = 0; j < _invArea._size._x; j++) {
			cout << std::setw(2) << _itemMasks[_invArea._size._x * i + j] << " | ";
		}
		cout << endl;
	}
	cout << "==========================" << endl << endl;
}