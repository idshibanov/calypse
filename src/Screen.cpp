#include <allegro5\allegro_primitives.h>
#include "Screen.h"

Screen::Screen(const JsonObject& entry) {
	_isMapScreen = false;
	_backColor = al_map_rgb(12, 41, 17);
}

Screen::~Screen() {

}

const std::vector<shared_ptr<UIFrame>>& Screen::getFrames() const {
	return _frames;
}

const std::vector<shared_ptr<UIElement>>& Screen::getElements() const {
	return _elements;
}

bool Screen::isMapScreen() const {
	return _isMapScreen;
}

void Screen::draw(const Point& displaySize) {
	if (_background) {
		_background->drawScaled(Point(0, 0), displaySize);
	} else {
		al_draw_filled_rectangle(0, 0, displaySize._x, displaySize._y, _backColor);
	}
}