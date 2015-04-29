#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "Frame.h"

UIFrame::UIFrame() : ScreenArea(Point(0,0), Point(0,0)) {
	_type = AREA_TYPE_FRAME;
	_visible = true;
	_draggable = false;
	_parent = 0;
}

UIFrame::UIFrame(const Point& pos, const Point& size, int zlevel, 
	             bool draggable, bool visible, UIFrame* parent)
				 : ScreenArea(pos, size) {
	_type = AREA_TYPE_FRAME;
	_zlevel = zlevel;
	_draggable = draggable;
	_visible = visible;
	_parent = parent;
}

UIFrame::~UIFrame() {

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
		al_draw_rectangle(_pos._x, _pos._y, last._x, last._y, al_map_rgb(160, 150, 93), 2);
	}
}