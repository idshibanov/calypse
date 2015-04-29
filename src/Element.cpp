#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "Element.h"
#include "Frame.h"
#include "Utils.h"

// Abstract UI Element class
UIElement::UIElement(Point pos, Point size, UIFrame* parent, bool active, bool visible)
	: ScreenArea(pos, size) {
	_parent = parent;
	_active = active;
	_visible = visible;
	_type = AREA_TYPE_ELEMENT;
	_elType = UIELEMENT_TYPE_INVALID;
}

UIElement::~UIElement() {

}

Point UIElement::getPos() {
	return _pos;
}

void UIElement::setPos(const Point& pos) {
	_pos = pos;
}

int UIElement::getElementType() {
	return _elType;
}

UIFrame* UIElement::getParent() {
	return _parent;
}

void UIElement::setParent(UIFrame* fr) {
	_parent = fr;
}

bool UIElement::isActive() {
	return _active;
}

bool UIElement::isVisible() {
	return _visible;
}

void UIElement::visibility(bool val) {
	_visible = val;
}


// Label class
UILabel::UILabel(Point pos, Point size, UIFrame* parent, shared_ptr<SpriteText> font,
	std::string& text, bool active, bool visible)
	: UIElement(pos, size, parent, active, visible) {
	_text = text;
	_font = font;
	_elType = UIELEMENT_TYPE_LABEL;
}

UILabel::~UILabel() {

}

std::string& UILabel::getText() {
	return _text;
}

void UILabel::update() {

}

void UILabel::draw() {
	Point drawPos(_pos);
	if (_parent) {
		drawPos += _parent->getAbsPos();
	}

	// add text positioning
	if (!_text.empty())
		_font->draw(_text, drawPos, al_map_rgb(20, 20, 20));
}


// Button class
UIButton::UIButton(Point pos, UIFrame* parent, int action_id, shared_ptr<SpriteText> font, 
                   std::string& text, bool active, bool visible, bool state, unsigned clickedTicks)
	               : UIElement(pos, Point(0,0), parent, active, visible), _clickTimer(clickedTicks) {
	_action_id = action_id;
	_font = font;
	_text = text; 
	_state = state;
	_elType = UIELEMENT_TYPE_BUTTON;

	int width = _font->getWidth(text);
	int height = _font->getHeight();
	_size.set(width + 30, height + 10);
}

UIButton::UIButton(Point pos, UIFrame* parent, Point size, int action_id, shared_ptr<Sprite> spr,
	shared_ptr<Sprite> sprOn, bool active, bool visible, bool state, unsigned clickedTicks)
	: UIElement(pos, size, parent, active, visible), _clickTimer(clickedTicks) {
	_action_id = action_id;
	_sprite = spr;
	_spriteClicked = sprOn;
	_state = state;
	_elType = UIELEMENT_TYPE_BUTTON;
}

UIButton::~UIButton() {

}

int UIButton::getActionID() {
	return _action_id;
}

bool UIButton::getState() {
	return _state;
}

std::string& UIButton::getText() {
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
	if (_visible) {
		Point drawPos(_pos);
		if (_parent) {
			drawPos += _parent->getAbsPos();
		}

		if (_sprite) {
			if (_clickTimer.isActive() && _spriteClicked) {
				_spriteClicked->draw(drawPos);
			} else {
				_sprite->draw(drawPos);
			}
		} else {
			al_draw_filled_rectangle(drawPos._x, drawPos._y, drawPos._x + _size._x, drawPos._y + _size._y, al_map_rgb(18, 43, 82));

			// add text positioning
			if (!_text.empty())
				_font->draw(_text, drawPos.add(15, 5), al_map_rgb(160, 150, 93));
		}
	}
}



CarouselMenu::CarouselMenu(Point pos, Point size, UIFrame* parent, bool active, bool visible)
	: UIElement(pos, size, parent, active, visible),
	_updateTimer(CAROUSEL_UPDATE), _progress(CAROUSEL_DURATION / CAROUSEL_UPDATE) {
	_spawnRate = 100;
	_lastItem = 0;
	_closing = false;
}

CarouselMenu::~CarouselMenu() {

}

void CarouselMenu::addOption(shared_ptr<UIButton> opt) {
	_options.push_back(opt);
	if (_options.size() > 1)
		_spawnRate = 100 / (_options.size() - 1);
}

void CarouselMenu::reset() {
	for (auto it = _options.begin(); it != _options.end(); it++) {
		it->get()->visibility(false);
	}
	_options.clear();
	_spawnRate = 0;
	_lastItem = 0;
	_closing = false;
	_updateTimer.relaunch();
	_progress.relaunch();
}

void CarouselMenu::update() {
	if (_updateTimer.check()) {
		animationStep();
		if (_closing) {
			if (!_progress.backwards()) {
				_updateTimer.relaunch();
			}
		} else {
			if (!_progress.check()) {
				_updateTimer.relaunch();
			}
		}
	}
}

void CarouselMenu::draw() {
	// do nothing
}

void CarouselMenu::closeAnimation() {
	if (!_progress.isActive()) {
		_closing = true;
	}
}

void CarouselMenu::spawnItem() {
	if (_lastItem < _options.size()) {
		_lastItem++;
		_options.at(_options.size() - _lastItem)->visibility(true);
	}
}

void CarouselMenu::animationStep() {
	if (_closing) {
		if (_closing && _lastItem > 0 && _progress.getProgress() <= _spawnRate*(_lastItem-1)) {
			_options.at(_options.size() - _lastItem)->visibility(false);
			_lastItem--;
		}
	} else {
		if (_spawnRate*_lastItem <= _progress.getProgress()) {
			spawnItem();
		}
	}

	for (auto it = _options.begin(); it != _options.end(); it++) {
		int idx = it - _options.begin();
		int order = abs(idx - (int)_options.size() + 1);
		if (order < _lastItem) {
			updateElement(idx, order);
		}
	}
}

double CarouselMenu::cubicBezierEasing(int max, int id) {
	double x = (_progress.getProgress() - id * _spawnRate) / 100;
	double mod = cubicBezierCurve(x, 1, 0.55, 0.45, 0);
	return max * mod;
}

void CarouselMenu::updateElement(int idx, int order) {
	double degrees = cubicBezierEasing(130, order) + 25;
	double angle = C_PI * degrees / 180;
	Point pos(std::sin(angle) * 65, -(std::cos(angle) * 65));

	_options.at(idx)->setPos(_pos + pos);
}
