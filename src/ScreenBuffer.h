#pragma once
#include <map>
#include <set>
#include <unordered_set>
#include "Element.h"
#include "Frame.h"
#include "ObjectArea.h"
#include "ItemArea.h"

// basically a frame 0 for now
// ordered map will be faster to iterate

class ScreenBuffer {
	std::unordered_set<shared_ptr<ScreenArea>> _items;
	std::unordered_set<shared_ptr<ScreenArea>>::iterator getElementArea(const Point&);
public:
	ScreenBuffer();
	~ScreenBuffer();
	void reset();
	void setElement(shared_ptr<ScreenArea>);
	shared_ptr<ScreenArea> getElement(const Point&);
	const std::unordered_set<shared_ptr<ScreenArea>>& getAllAreas() const;
};