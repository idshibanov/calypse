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
	shared_ptr<ResourceCtl> _res;
	bool _draggable;
	bool _visible;
	UIFrame* _parent;
	std::vector<shared_ptr<UIElement>> _elements;
public:
	UIFrame(const Point& pos, const Point& size, shared_ptr<ResourceCtl> res, const std::string& title,
		    int zlevel = 100, bool draggable = false, bool visible = false, UIFrame* parent = 0);
	virtual ~UIFrame();
	Point getPos() const;
	Point getAbsPos() const;
	int getZLevel() const;
	bool isVisible() const;
	void move(const Point& delta);
	void setParent(UIFrame* p);
	void setZLevel(unsigned z);
	void setVisible(bool value);
	virtual void addElement(shared_ptr<UIElement> el);
	virtual std::vector<shared_ptr<UIElement>> getElements() const;
	virtual void draw();
};


class ObjectInfoFrame : public UIFrame {
	shared_ptr<ActorState> _state;
	shared_ptr<UILabel> _statLabels[STAT_SCORE_LAST];
	shared_ptr<UILabel> _skillLabels[SKILL_SCORE_LAST];
	std::vector<shared_ptr<UILabel>> _inventory;
public:
	ObjectInfoFrame(const Point& pos, const Point& size, shared_ptr<ResourceCtl> res,
		            const std::string& title, shared_ptr<ActorState> state);
	~ObjectInfoFrame();
	std::vector<shared_ptr<UIElement>> getElements() const;
	void draw();
};


class ContainerFrame : public UIFrame {
	shared_ptr<Inventory> _inv;
	shared_ptr<ContainerArea> _contArea;
public:
	ContainerFrame(const Point& pos, shared_ptr<ResourceCtl> res,
		const std::string& title, shared_ptr<Inventory> inv);
	~ContainerFrame();
	std::vector<shared_ptr<UIElement>> getElements() const;
	void draw();
};