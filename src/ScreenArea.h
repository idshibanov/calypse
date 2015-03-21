#pragma once
#include "Sprite.h"

enum ScreenAreaType {
	AREA_TYPE_SCREEN,
	AREA_TYPE_OBJECT,
	AREA_TYPE_ELEMENT,
	AREA_TYPE_FRAME,
	AREA_TYPE_INVALID
};

class ScreenArea {
protected:
	ScreenAreaType _type;
public:
	Point _pos;
	Point _size;
	int _zlevel;
	ScreenArea(const Point& pos, const Point& size);
	virtual ~ScreenArea();
	ScreenAreaType getType() const;
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