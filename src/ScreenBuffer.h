#pragma once
#include <map>
#include "Object.h"
#include "ScreenArea.h"

// basically a frame 0 for now
// ordered map will be faster to iterate

class ScreenBuffer {
	map<ScreenArea, shared_ptr<MapObject>> _items;
public:
	ScreenBuffer();
	~ScreenBuffer();
	void reset();
	void setElement(ScreenArea, shared_ptr<MapObject>);
	const shared_ptr<ScreenArea> getElementArea(const Point&);
	shared_ptr<MapObject> getElement(const Point&);
	shared_ptr<MapObject> getElement(const ScreenArea&);
};