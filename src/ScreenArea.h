#pragma once
#include "Sprite.h"

class ScreenArea {
public:
	Point _pos;
	Point _size;
	int _zlevel;
	ScreenArea(const Point& pos, const Point& size);
	virtual ~ScreenArea();
	Point getMax() const;
	bool operator< (const ScreenArea& area) const;
	bool operator< (const Point& pos) const;
	bool operator== (const ScreenArea&) const;
	size_t operator() (const ScreenArea&) const;
};

template <>
struct hash<shared_ptr<ScreenArea>> {
	size_t operator() (const shared_ptr<ScreenArea>& area) const {
		return area->_pos.toRenderPriority();
	}
};