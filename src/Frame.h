#pragma once
#include "ResourceCtl.h"
#include "Element.h"
#include "ObjectState.h"

class UIFrame : public ScreenArea {
	UIFrame() = delete;
	UIFrame(const UIFrame&) = delete;
	UIFrame& operator=(const UIFrame&) = delete;
protected:
	int _titleHeight;
	weak_ptr<ResourceCtl> _res;
	bool _draggable;
	bool _visible;
	UIFrame* _parent;
	std::vector<shared_ptr<UIElement>> _elements;
public:
	UIFrame(const Point& pos, const Point& size, weak_ptr<ResourceCtl> res, const std::string& title, 
		    int zlevel = 1, bool draggable = false, bool visible = false, UIFrame* parent = 0);
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
	virtual void draw();
};


class ObjectInfoFrame : public UIFrame {
	shared_ptr<ActorState> _state;
	shared_ptr<UILabel> _statLabels[STAT_SCORE_LAST];
	shared_ptr<UILabel> _skillLabels[SKILL_SCORE_LAST];
public:
	ObjectInfoFrame(const Point& pos, const Point& size, weak_ptr<ResourceCtl> res, 
		            const std::string& title, shared_ptr<ActorState> state);
	~ObjectInfoFrame();
	void draw();
};