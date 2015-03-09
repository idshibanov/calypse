#pragma once
#include <map>
#include <set>
#include <unordered_set>
#include "ScreenArea.h"

// basically a frame 0 for now
// ordered map will be faster to iterate

class ScreenBuffer {
	set<ScreenArea> _items;
	set<ScreenArea>::iterator getElementArea(const Point&);
public:
	ScreenBuffer();
	~ScreenBuffer();
	void reset();
	void setElement(ScreenArea);
	shared_ptr<MapObject> getElement(const Point&);
	shared_ptr<MapObject> getElement(const ScreenArea&);
};