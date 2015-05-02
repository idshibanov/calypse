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
	Point _pos;
	Point _size;
	int _zlevel;
	ScreenAreaType _type;
public:
	ScreenArea(const Point& pos, const Point& size);
	virtual ~ScreenArea();
	ScreenAreaType getType() const;
	virtual Point getPos() const;
	Point getMax() const;
	int getZlevel() const;
	void setZlevel(int z);
	bool operator< (const ScreenArea& area) const;
	bool operator< (const Point& pos) const;
	bool operator== (const ScreenArea&) const;
	size_t operator() (const ScreenArea&) const;
};

namespace std {
	template <>
	struct hash<shared_ptr<ScreenArea>> {
		size_t operator() (const shared_ptr<ScreenArea>& area) const {
			return area->getPos().toRenderPriority();
		}
	};
}