#pragma once
#include "ScreenArea.h"

class UIFrame : public ScreenArea {
	bool _draggable;
	bool _visible;
	UIFrame* _parent;
public:
	UIFrame();
	UIFrame(const Point& pos, const Point& size, int zlevel = 0, 
		    bool draggable = false, bool visible = true, UIFrame* parent = 0);
	virtual ~UIFrame();
	Point getPos() const;
	Point getAbsPos() const;
	int getZLevel() const;
	bool isVisible() const;
	void move(const Point& pos);
	void setParent(UIFrame* p);
	void setZLevel(unsigned z);
	void setVisible(bool value);
	void draw();
};