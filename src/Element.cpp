#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "element.h"

// Abstract UI Element class
UIElement::UIElement(Point pos, Point size, UIFrame* parent, bool active, bool visible) {
	_pos = pos;
	_size = size;
	_parent = parent;
	_active = active;
	_visible = visible;
	_type = UIELEMENT_TYPE_INVALID;
}

UIElement::~UIElement() {

}

Point UIElement::getPos() {
	return _pos;
}

Point UIElement::getMax() {
	return _pos + _size;
}

int UIElement::getType() {
	return _type;
}

bool UIElement::isActive() {
	return _active;
}

bool UIElement::isVisible() {
	return _visible;
}

// Label class
UILabel::UILabel(Point pos, Point size, UIFrame* parent, shared_ptr<SpriteText> font,
	string& text, bool active, bool visible)
	: UIElement(pos, size, parent, active, visible) {
	_text = text;
	_font = font;
	_type = UIELEMENT_TYPE_LABEL;
}

UILabel::~UILabel() {

}

string& UILabel::getText() {
	return _text;
}

void UILabel::update() {

}

void UILabel::draw() {
	Point drawPos(_pos);
	if (_parent) {
		drawPos.add(_parent->getAbsXPos(), _parent->getAbsYPos());
	}

	// add text positioning
	if (!_text.empty())
		_font->draw(_text, drawPos, al_map_rgb(20, 20, 20));
}


// Button class
UIButton::UIButton(Point pos, UIFrame* parent, Point size, int action_id, shared_ptr<Sprite> spr,
	shared_ptr<Sprite> sprOn, shared_ptr<SpriteText> font, string& text,
	bool active, bool visible, bool state, unsigned clickedTicks)
	: UIElement(pos, size, parent, active, visible), _clickTimer(clickedTicks) {
	_action_id = action_id;
	_sprite = spr;
	_spriteClicked = sprOn;
	_text = text;
	_state = state;
	_font = font;
	_type = UIELEMENT_TYPE_BUTTON;
}

UIButton::~UIButton() {

}

int UIButton::getActionID() {
	return _action_id;
}

bool UIButton::getState() {
	return _state;
}

string& UIButton::getText() {
	return _text;
}

void UIButton::launchTimer() {
	_clickTimer.relaunch();
}

bool UIButton::checkTimer() {
	if (_clickTimer.isActive() == false)
		_clickTimer.relaunch();

	return _clickTimer.check();
}

void UIButton::update() {
	_clickTimer.check();
}

void UIButton::draw() {
	Point drawPos(_pos);
	if (_parent) {
		drawPos.add(_parent->getAbsXPos(), _parent->getAbsYPos());
	}

	if (_sprite) {
		if (_clickTimer.isActive() && _spriteClicked) {
			_spriteClicked->draw(drawPos);
		} else {
			_sprite->draw(drawPos);
		}
	} else {
		al_draw_filled_rectangle(drawPos._x, drawPos._y, drawPos._x + _size._x, drawPos._y + _size._y, al_map_rgb(230, 30, 30));
	}

	// add text positioning
	if (!_text.empty())
		_font->draw(_text, drawPos, al_map_rgb(20, 20, 20));
}