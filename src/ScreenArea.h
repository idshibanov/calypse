#pragma once
#include "Sprite.h"
#include "Object.h"

class ScreenArea {
public:
	Point _pos;
	Point _size;
	int _zlevel;
};

class ObjectArea : public ScreenArea {
public:
	shared_ptr<MapObject> _obj;
	SpriteSheet* _sprite;
	ObjectArea(const Point&, const Point&, shared_ptr<MapObject>, SpriteSheet*);
	~ObjectArea();
	Point getMax() const;
	bool operator< (const ObjectArea& area) const;
	bool operator< (const Point& pos) const;
	bool operator== (const ObjectArea&) const;
	size_t operator() (const ObjectArea&) const;
};

template <>
struct hash<ObjectArea> {
	size_t operator() (const ObjectArea& area) const {
		return area._pos.toRenderPriority();
	}
};