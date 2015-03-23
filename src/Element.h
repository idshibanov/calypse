#pragma once
#include <string>
#include "ScreenArea.h"
#include "Main.h"
#include "Frame.h"
#include "Timer.h"

#define UIBUTTON_DEFAULT_CLICKED_TICKS 10

enum UIElementType {
	UIELEMENT_TYPE_BUTTON,
	UIELEMENT_TYPE_LABEL,
	UIELEMENT_TYPE_INVALID
};

class UIElement : public ScreenArea {
protected:
	UIFrame* _parent;
	bool _active;
	bool _visible;
	UIElementType _elType;
	UIElement(Point pos, Point size, UIFrame* parent, bool active = true, bool visible = true);
public:
	virtual ~UIElement();
	Point getPos();
	int getElementType();
	bool isActive();
	bool isVisible();
	virtual void update() = 0;
	virtual void draw() = 0;
};

class UILabel : public UIElement {
	std::string _text;
	std::shared_ptr<SpriteText> _font;
public:
	UILabel(Point pos, Point size, UIFrame* parent, std::shared_ptr<SpriteText> font,
		std::string& text, bool active = true, bool visible = true);
	~UILabel();
	std::string& getText();
	void update();
	void draw();
};

class UIButton : public UIElement {
	int _action_id;
	bool _state;
	std::shared_ptr<Sprite> _sprite;
	std::shared_ptr<Sprite> _spriteClicked;
	TaskTimer _clickTimer;
	std::string _text;
	std::shared_ptr<SpriteText> _font;
public:
	UIButton(Point pos, UIFrame* parent, Point size, int action_id, std::shared_ptr<Sprite> spr,
		std::shared_ptr<Sprite> sprOn, std::shared_ptr<SpriteText> font, std::string& text,
		bool active = true, bool visible = true, bool state = false,
		unsigned clickedTicks = UIBUTTON_DEFAULT_CLICKED_TICKS);
	~UIButton();
	int getActionID();
	bool getState();
	std::string& getText();
	void launchTimer();
	bool checkTimer();
	void update();
	void draw();
};