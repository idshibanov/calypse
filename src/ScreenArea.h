#pragma once
#include "Sprite.h"
#include "Object.h"

class ScreenArea {
public:
	Point _pos;
	Point _size;
	int _zlevel;
	shared_ptr<MapObject> _obj;
	weak_ptr<Sprite> _sprite;
	ScreenArea(const Point&, const Point&, shared_ptr<MapObject>, weak_ptr<Sprite>);
	~ScreenArea();
	Point getMax() const;
	bool operator< (const ScreenArea& area) const;
	bool operator< (const Point& pos) const;
	size_t operator() (const ScreenArea&) const;
};