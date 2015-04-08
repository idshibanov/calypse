#pragma once
#include "ScreenArea.h"
#include "Object.h"
#include "ActionType.h"

struct ObjectActionArea {
	Rect _area;
	std::vector<ActionType> _acts;
};

class ObjectArea : public ScreenArea {
public:
	std::vector<shared_ptr<ObjectActionArea>> _subAreas;
	std::shared_ptr<MapObject> _obj;
	weak_ptr<Sprite> _sprite;
	ObjectArea(const Point&, const Point&, std::shared_ptr<MapObject>, weak_ptr<Sprite>);
	~ObjectArea();
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