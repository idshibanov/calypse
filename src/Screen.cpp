#include <allegro5\allegro_primitives.h>
#include "Screen.h"

Screen::Screen(const JsonObject& entry) {
	bool status = true;

	_backColor = extractValue<ALLEGRO_COLOR>(entry.getValue("backColor"), &status);
	if (!status) {
		_backColor = al_map_rgb(12, 41, 17);
		status = true;
	}

	_isMapScreen = extractValue<bool>(entry.getValue("map"), &status);
	if (!status) {
		_isMapScreen = false;
		status = true;
	}

	std::string name = extractValue<std::string>(entry.getValue("name"));
	auto frameArr = std::dynamic_pointer_cast<JsonTValue<std::vector<shared_ptr<JsonValue>>>>(entry.getValue("frames"));
	auto elemArr = std::dynamic_pointer_cast<JsonTValue<std::vector<shared_ptr<JsonValue>>>>(entry.getValue("elements"));

	if (frameArr) {
		
	}
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