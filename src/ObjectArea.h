#pragma once
#include "Object.h"
#include "ObjectInfo.h"
#include "ScreenArea.h"

class ObjectArea : public ScreenArea {
	shared_ptr<ObjectInfo> _info;
	weak_ptr<Sprite> _sprite;
public:
	shared_ptr<MapObject> _obj;
	ObjectArea(const Point&, const Point&, shared_ptr<MapObject>, weak_ptr<Sprite>, shared_ptr<ObjectInfo>);
	virtual ~ObjectArea();
	shared_ptr<SpriteSheet> castSprite() const;
	const shared_ptr<ObjectActionArea> getSubArea(const Point& pos) const;
};

/*
template <>
struct hash<ObjectArea> {
	size_t operator() (const ObjectArea& area) const {
		return area._pos.toRenderPriority();
	}
};
*/