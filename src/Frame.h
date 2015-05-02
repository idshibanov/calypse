#pragma once
#include "Element.h"

class UIFrame : public ScreenArea {
	bool _draggable;
	bool _visible;
	UIFrame* _parent;
	std::vector<shared_ptr<UIElement>> _elements;
public:
	UIFrame();
	UIFrame(const Point& pos, const Point& size, int zlevel = 1, 
		    bool draggable = false, bool visible = true, UIFrame* parent = 0);
	virtual ~UIFrame();
	void addElement(shared_ptr<UIElement> el);
	std::vector<shared_ptr<UIElement>> getElements() const;
	Point getPos() const;
	Point getAbsPos() const;
	int getZLevel() const;
	bool isVisible() const;
	void move(const Point& delta);
	void setParent(UIFrame* p);
	void setZLevel(unsigned z);
	void setVisible(bool value);
	void draw();
};