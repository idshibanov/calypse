#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "frame.h"
#include "element.h"

UIFrame::UIFrame() {
	_type = UIFrame_TYPE_INVALID;
	_xpos = 0;
	_ypos = 0;
	_xsize = 0;
	_ysize = 0;
	_zlevel = 0;
	_draggable = false;
	_parent = 0;
}

UIFrame::UIFrame(UIFrameType type, int xpos, int ypos, unsigned xsize, unsigned ysize,
	int zlevel, bool draggable, bool visible, UIFrame* parent) {
	_type = type;
	_xpos = xpos;
	_ypos = ypos;
	_xsize = xsize;
	_ysize = ysize;
	_zlevel = zlevel;
	_draggable = draggable;
	_visible = visible;
	_parent = parent;
}

UIFrame::~UIFrame() {

}

int UIFrame::getXPos() {
	return _xpos;
}

int UIFrame::getYPos() {
	return _ypos;
}

int UIFrame::getXMaxPos() {
	return _xpos + _xsize;
}

int UIFrame::getYMaxPos() {
	return _ypos + _ysize;
}

int UIFrame::getZLevel() {
	return _zlevel;
}

UIFrameType UIFrame::getType() {
	return _type;
}

int UIFrame::getAbsXPos() {
	int retval = _xpos;
	if (_parent != NULL)
		retval += _parent->getAbsXPos();
	return retval;
}

int UIFrame::getAbsYPos() {
	int retval = _ypos;
	if (_parent != NULL)
		retval += _parent->getAbsYPos();
	return retval;
}

bool UIFrame::isVisible() {
	return _visible;
}

void UIFrame::move(int xpos, int ypos) {
	_xpos = xpos;
	_ypos = ypos;
}

void UIFrame::addElement(shared_ptr<UIElement> elem) {
	if (elem != NULL)
		_elem.push_back(elem);
}

shared_ptr<UIElement> UIFrame::getElement(int absX, int absY) {
	shared_ptr<UIElement> retval = NULL;
	bool found = false;
	int frame_X = getAbsXPos();
	int frame_Y = getAbsYPos();
	for (auto it = _elem.begin(); !found && it != _elem.end(); ++it) {
		Point p1 = (*it)->getPos();
		Point p2 = (*it)->getMax();
		if (absX >= (frame_X + p1._x) && absY >= (frame_Y + p1._y)) {
			if (absX <= (frame_X + p2._x) && absY <= (frame_Y + p2._y)) {
				found = true;
				retval = *it;
			}
		}
	}
	return retval;
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

void UIFrame::setSprite(shared_ptr<Sprite> sprite) {
	_bgSprite = sprite;
}

void UIFrame::draw() {
	if (_visible) {
		if (_bgSprite == NULL) {
			al_draw_filled_rectangle(_xpos, _ypos, _xpos + _xsize, _ypos + _ysize, al_map_rgb(230, 190, 150));
		} else {
			_bgSprite->drawScaled(Point(_xpos, _ypos), Point(_xsize, _ysize));
		}

		for (auto it = _elem.begin(); it != _elem.end(); ++it) {
			(*it)->draw();
		}
	}
}