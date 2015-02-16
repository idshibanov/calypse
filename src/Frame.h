#pragma once
#include <vector>
#include "main.h"
#include "sprite.h"

class UIElement;

enum UIFrameType {
	UIFrame_TYPE_VIEWPORT,
	UIFrame_TYPE_MENU,
	UIFrame_TYPE_INVALID
};

class UIFrame {
	UIFrameType _type;
	unsigned _xsize;
	unsigned _ysize;
	int _xpos;
	int _ypos;
	int _zlevel;
	bool _draggable;
	bool _visible;
	UIFrame* _parent;

	vector<shared_ptr<UIElement>> _elem;
	shared_ptr<Sprite> _bgSprite;
	ALLEGRO_COLOR _bgColor;
public:
	UIFrame();
	UIFrame(UIFrameType type, int xpos, int ypos, unsigned xsize, unsigned ysize,
		int zlevel = 0, bool draggable = false, bool visible = true, UIFrame* parent = 0);
	virtual ~UIFrame();
	int getXPos();
	int getYPos();
	int getXMaxPos();
	int getYMaxPos();
	int getZLevel();
	UIFrameType getType();
	int getAbsXPos();
	int getAbsYPos();
	bool isVisible();
	void move(int xpos, int ypos);
	void addElement(shared_ptr<UIElement> elem);
	shared_ptr<UIElement> getElement(int absX, int absY);
	void setParent(UIFrame* p);
	void setZLevel(unsigned z);
	void setVisible(bool value);
	void setSprite(shared_ptr<Sprite> sprite);
	void draw();
};