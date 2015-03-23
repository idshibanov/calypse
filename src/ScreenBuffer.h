#pragma once
#include <map>
#include <set>
#include <unordered_set>
#include "Element.h"
#include "ObjectArea.h"

// basically a frame 0 for now
// ordered map will be faster to iterate

class ScreenBuffer {
	std::unordered_set<std::shared_ptr<ScreenArea>> _items;
	std::unordered_set<std::shared_ptr<ScreenArea>>::iterator getElementArea(const Point&);
public:
	ScreenBuffer();
	~ScreenBuffer();
	void reset();
	void setElement(std::shared_ptr<ScreenArea>);
	std::shared_ptr<ScreenArea> getElement(const Point&);
};