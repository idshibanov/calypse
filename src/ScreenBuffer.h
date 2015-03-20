#pragma once
#include <map>
#include <set>
#include <unordered_set>
#include "ScreenArea.h"

// basically a frame 0 for now
// ordered map will be faster to iterate

class ScreenBuffer {
	unordered_set<ObjectArea> _items;
	unordered_set<ObjectArea>::iterator getElementArea(const Point&);
public:
	ScreenBuffer();
	~ScreenBuffer();
	void reset();
	void setElement(ObjectArea);
	shared_ptr<MapObject> getElement(const Point&);
	shared_ptr<MapObject> getElement(const ObjectArea&);
};