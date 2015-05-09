#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "Frame.h"

UIFrame::UIFrame(const Point& pos, const Point& size, weak_ptr<ResourceCtl> res, const std::string& title,
	             int zlevel, bool draggable, bool visible, UIFrame* parent) : ScreenArea(pos, size) {

	_res = res;
	auto resCtl = _res.lock();

	if (resCtl && !title.empty()) {
		auto font = resCtl->getFont(12);

		_titleHeight = font->getHeight() + 4; // padding
		int titleWidth = font->getWidth(title) + 10; // padding

		int buttonWidth = 20;
		int totalWidth = titleWidth + buttonWidth;

		_size._y += titleWidth;
		if (_size._x < totalWidth) {
			_size._x = totalWidth;
		} else {
			totalWidth = _size._x;
		}

		auto closeFrame = make_shared<UIButton>(Point(totalWidth - buttonWidth, 2), this, ACTION_CLOSE_PARENT, font, std::string("X"));
		ElementStyle closeStyle;
		closeStyle._border = false;
		closeStyle._background = false;
		closeStyle._padding = Point(5, 0);
		closeStyle._textColor = al_map_rgb(255,255,255);
		closeFrame->setStyle(closeStyle);
		addElement(closeFrame);

		auto frameTitle = make_shared<UILabel>(Point((totalWidth - titleWidth) / 2, 2), this, font, title);
		addElement(frameTitle);
	}

	_type = AREA_TYPE_FRAME;
	_draggable = draggable;
	_visible = visible;
	_parent = parent;
	_res = res;
}

UIFrame::~UIFrame() {

}

void UIFrame::addElement(shared_ptr<UIElement> el) {
	el->setParent(this);
	el->setZlevel(_zlevel + 1);
	_elements.push_back(el);
}

std::vector<shared_ptr<UIElement>> UIFrame::getElements() const {
	return _elements;
}

Point UIFrame::getPos() const {
	return _pos;
}

Point UIFrame::getAbsPos() const {
	Point retval = _pos;
	if (_parent != NULL)
		retval += _parent->getAbsPos();
	return retval;
}


int UIFrame::getZLevel() const {
	return _zlevel;
}

bool UIFrame::isVisible() const {
	return _visible;
}

void UIFrame::move(const Point& delta) {
	_pos += delta;
}

void UIFrame::setParent(UIFrame* p) {
	_parent = p;
}

void UIFrame::setZLevel(unsigned z) {
	_zlevel = z;
}

void UIFrame::setVisible(bool value) {
	_visible = value;
}

void UIFrame::draw() {
	if (_visible) {
		Point last = _pos + _size;
		al_draw_filled_rectangle(_pos._x, _pos._y, last._x, last._y, al_map_rgb(18, 43, 82));
		al_draw_filled_rectangle(_pos._x, _pos._y, last._x, _pos._y + _titleHeight, al_map_rgb(160, 150, 93));
		al_draw_rectangle(_pos._x, _pos._y, last._x, last._y, al_map_rgb(160, 150, 93), 2);

		for (auto it = _elements.begin(); it != _elements.end(); it++) {
			(*(*it)).draw();
		}
	}
}



ObjectInfoFrame::ObjectInfoFrame(const Point& pos, const Point& size, weak_ptr<ResourceCtl> res, const std::string& title,
                                 shared_ptr<ActorState> state) : UIFrame(pos, size, res, title) {
	_state = state;
	auto resCtl = _res.lock();
	if (resCtl) {
		auto font = resCtl->getFont(10);

		if (font) {
			for (int i = 0; i < STAT_SCORE_LAST; i++) {
				_statLabels[i] = make_shared<UILabel>(Point(10, _titleHeight + (i * 15)), this, font, resCtl->getStatName((StatScoreID)i));
			}

			for (int i = 0; i < SKILL_SCORE_LAST; i++) {
				_skillLabels[i] = make_shared<UILabel>(Point(100, _titleHeight + (i * 15)), this, font, resCtl->getSkillName((SkillScoreID)i));
			}

			int offset = _titleHeight + (SKILL_SCORE_LAST + 2) * 15;
			for (int i = 0; i < ITEM_ID_LAST; i++) {
				_inventory[i] = make_shared<UILabel>(Point(100, offset + (i * 15)), this, font, resCtl->getItemName((ItemID)i));
			}
		}
	}
}

ObjectInfoFrame::~ObjectInfoFrame() {

}

void ObjectInfoFrame::draw() {
	UIFrame::draw();

	for (int i = 0; i < STAT_SCORE_LAST; i++) {
		if (_statLabels[i]) {
			_statLabels[i]->draw(std::to_string(_state->getStat((StatScoreID)i)));
		}
	}

	for (int i = 0; i < SKILL_SCORE_LAST; i++) {
		if (_skillLabels[i]) {
			_skillLabels[i]->draw(std::to_string(_state->getSkill((SkillScoreID)i)));
		}
	}

	for (int i = 0; i < ITEM_ID_LAST; i++) {
		if (_inventory[i]) {
			_inventory[i]->draw(std::to_string(_state->getItemCount((ItemID)i)));
		}
	}
}