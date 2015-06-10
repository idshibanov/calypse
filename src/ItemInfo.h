#pragma once
#include <string>
#include "ItemType.h"
#include "Point.h"

class ItemInfo {
	int _type;
	std::string _name;
	Point _invSize;
	Point _offset;
	Point _sprSize;
	int _spriteID;
	int _frame;
public:
	ItemInfo(int, const std::string&, const Point&, const Point&, const Point&, int, int);
	ItemInfo(const ItemInfo&);
	virtual ~ItemInfo();
	ItemInfo& operator=(const ItemInfo&);
	int type() const;
	Point invSize() const;
	Point offset() const;
	Point sprSize() const;
	int spriteID() const;
	int frame() const;
	std::string name() const;
};