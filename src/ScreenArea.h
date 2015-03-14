#pragma once
#include "Sprite.h"
#include "Object.h"

class ScreenArea {
public:
	Point _pos;
	Point _size;
	int _zlevel;
	shared_ptr<MapObject> _obj;
	SpriteSheet* _sprite;
	ScreenArea(const Point&, const Point&, shared_ptr<MapObject>, SpriteSheet*);
	~ScreenArea();
	Point getMax() const;
	bool operator< (const ScreenArea& area) const;
	bool operator< (const Point& pos) const;
	bool operator== (const ScreenArea&) const;
	size_t operator() (const ScreenArea&) const;
};

template <>
struct hash<ScreenArea> {
	size_t operator() (const ScreenArea& area) const {
		return area._pos.toRenderPriority();
	}
};