#pragma once
#include <map>
#include <set>
#include "ScreenArea.h"

// basically a frame 0 for now
// ordered map will be faster to iterate

class ScreenBuffer {
	set<ScreenArea> _items;
public:
	ScreenBuffer();
	~ScreenBuffer();
	void reset();
	void setElement(ScreenArea);
	const shared_ptr<ScreenArea> getElementArea(const Point&);
	shared_ptr<MapObject> getElement(const Point&);
	shared_ptr<MapObject> getElement(const ScreenArea&);
};