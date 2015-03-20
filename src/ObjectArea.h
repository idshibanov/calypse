#pragma once
#include "ScreenArea.h"
#include "Object.h"

class ObjectArea : public ScreenArea {
public:
	shared_ptr<MapObject> _obj;
	SpriteSheet* _sprite;
	ObjectArea(const Point&, const Point&, shared_ptr<MapObject>, SpriteSheet*);
	~ObjectArea();
};

/*
template <>
struct hash<ObjectArea> {
	size_t operator() (const ObjectArea& area) const {
		return area._pos.toRenderPriority();
	}
};
*/