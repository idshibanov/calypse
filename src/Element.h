#pragma once
#include <string>
#include <vector>
#include "ScreenArea.h"
#include "Main.h"
#include "Timer.h"
#include "ElementStyle.h"

#define UIBUTTON_DEFAULT_CLICKED_TICKS 10

class UIFrame;

enum UIElementType {
	UIELEMENT_TYPE_BUTTON,
	UIELEMENT_TYPE_LABEL,
	UIELEMENT_TYPE_INVALID
};

class UIElement : public ScreenArea {
	UIElement() = delete;
	UIElement(const UIElement&) = delete;
	UIElement& operator=(const UIElement&) = delete;
protected:
	UIFrame* _parent;
	bool _visible;
	UIElementType _elType;
	UIElement(Point pos, UIFrame* parent, bool visible = true);
	UIElement(Point pos, Point size, UIFrame* parent, bool visible = true);
public:
	virtual ~UIElement();
	Point getPos() const;
	void setPos(const Point& pos);
	int getElementType();
	UIFrame* getParent();
	void setParent(UIFrame*);
	bool isVisible();
	void visibility(bool val);
	virtual void update() = 0;
	virtual void draw() = 0;
};

class UILabel : public UIElement {
	std::string _text;
	shared_ptr<SpriteText> _font;
public:
	UILabel(Point pos, UIFrame* parent, shared_ptr<SpriteText> font, const std::string& text, bool visible = true);
	~UILabel();
	std::string& getText();
	void setText(const std::string& text);
	void update();
	void draw();
	void draw(const std::string& value);
};

class UIButton : public UIElement {
	bool _active;
	int _action_id;
	bool _state;
	shared_ptr<Sprite> _sprite;
	shared_ptr<Sprite> _spriteClicked;
	TaskTimer _clickTimer;
	std::string _text;
	shared_ptr<SpriteText> _font;
	ElementStyle _style;
public:
	UIButton(Point pos, UIFrame* parent, Point size, int action_id, shared_ptr<Sprite> spr,
	         shared_ptr<Sprite> sprOn, bool active = true, bool visible = true, bool state = false,
	         unsigned clickedTicks = UIBUTTON_DEFAULT_CLICKED_TICKS);
	UIButton(Point pos, UIFrame* parent, int action_id, shared_ptr<SpriteText> font, std::string& text,
	         bool active = true, bool visible = true, bool state = false,
	         unsigned clickedTicks = UIBUTTON_DEFAULT_CLICKED_TICKS);
	~UIButton();
	bool isActive();
	int getActionID();
	bool getState();
	std::string& getText();
	void setStyle(const ElementStyle& st);
	void launchTimer();
	bool checkTimer();
	void update();
	void draw();
};

#define CAROUSEL_UPDATE 20
#define CAROUSEL_DURATION 800

class CarouselMenu : public UIElement {
	std::vector<shared_ptr<UIButton>> _options;
	TaskTimer _updateTimer;
	TaskTimer _progress;
	double _spawnRate;
	int _lastItem;
	bool _closing;

	void animationStep();
	void spawnItem();
	double cubicBezierEasing(int max, int id);
	void updateElement(int idx, int order);
public:
	CarouselMenu(Point pos, Point size, UIFrame* parent, bool visible = true);
	~CarouselMenu();
	void addOption(shared_ptr<UIButton> opt);
	void reset();
	void update();
	void draw();
	void closeAnimation();
};
