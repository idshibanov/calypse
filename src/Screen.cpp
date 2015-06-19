#include <allegro5\allegro_primitives.h>
#include "Screen.h"

Screen::Screen(shared_ptr<ResourceCtl> res, const JsonObject& entry) {
	_res = res;
	bool status = true;

	_backColor = extractWithDefault<ALLEGRO_COLOR>(entry.getValue("backColor"), al_map_rgb(12, 41, 17));
	_isMapScreen = extractWithDefault<bool>(entry.getValue("map"), false);

	std::string name = extractValue<std::string>(entry.getValue("name"));
	auto framePtr = std::dynamic_pointer_cast<JsonTValue<std::vector<shared_ptr<JsonValue>>>>(entry.getValue("frames"));
	auto elemPtr = std::dynamic_pointer_cast<JsonTValue<std::vector<shared_ptr<JsonValue>>>>(entry.getValue("elements"));

	if (framePtr) {
		auto frameArr = framePtr->getValue();
		for (auto tmp : frameArr) {
			_addFrame(std::dynamic_pointer_cast<JsonObject>(tmp));
		}
	}
	if (elemPtr) {
		auto elemArr = elemPtr->getValue();
		for (auto tmp : elemArr) {
			_addElement(std::dynamic_pointer_cast<JsonObject>(tmp));
		}
	}
}

Screen::~Screen() {

}

bool Screen::_addFrame(shared_ptr<JsonObject> entry) {
	if (entry) {
		bool valid = true;

		std::string type = extractValue<std::string>(entry->getValue("type"), &valid);
		Point pos = extractValue<Point>(entry->getValue("pos"), &valid);
		Point size = extractValue<Point>(entry->getValue("size"));
		std::string title = extractValue<std::string>(entry->getValue("title"));

		if (valid) {
			if (type.compare("Container") != 0 && type.compare("ObjectInfo") != 0) {
				auto frame = make_shared<UIFrame>(pos, size, _res, title);
				
				auto subElems = std::dynamic_pointer_cast<JsonTValue<std::vector<shared_ptr<JsonValue>>>>(entry->getValue("elements"));
				if (subElems) {
					auto children = subElems->getValue();
					for (auto tmp : children) {
						_addElement(std::dynamic_pointer_cast<JsonObject>(tmp), frame.get());
					}
				}
				_frames.push_back(frame);
				return true;
			}
		}
	}
	return false;
}

bool Screen::_addElement(shared_ptr<JsonObject> entry, UIFrame* parent) {
	if (entry) {
		bool valid = true;

		std::string type = extractValue<std::string>(entry->getValue("type"), &valid);
		Point pos = extractValue<Point>(entry->getValue("pos"), &valid);


		if (valid) {
			if (type.compare("Label") == 0) {
				int fontSize = extractValue<int>(entry->getValue("fontSize"), &valid);
				std::string text = extractValue<std::string>(entry->getValue("text"), &valid);

				if (valid) {
					_elements.push_back(make_shared<UILabel>(pos, parent, _res->getFont(fontSize), text));
					return true;
				}
			} else if (type.compare("Button") == 0) {

				int action = extractWithDefault<int>(entry->getValue("action"), -1);
				bool active = extractWithDefault<bool>(entry->getValue("active"), true);
				bool visible = extractWithDefault<bool>(entry->getValue("visible"), true);
				bool state = extractWithDefault<bool>(entry->getValue("state"), false);

				int fontSize = extractValue<int>(entry->getValue("fontSize"), &valid);
				std::string text = extractValue<std::string>(entry->getValue("text"), &valid);
				
				if (valid) {
					_elements.push_back(make_shared<UIButton>(pos, parent, action, _res->getFont(fontSize), text, active, visible, state));
					return true;
				} else {
					valid = true;

					Point size = extractValue<Point>(entry->getValue("size"), &valid);
					std::string spr = extractValue<std::string>(entry->getValue("sprite"), &valid);
					std::string sprOn = extractValue<std::string>(entry->getValue("spriteOn"), &valid);

					if (valid) {
						auto sprPtr = _res->getSprite(spr.c_str());
						auto sprOnPtr = _res->getSprite(sprOn.c_str());
						_elements.push_back(make_shared<UIButton>(pos, parent, size, action, sprPtr, sprOnPtr, active, visible, state));
						return true;
					}
				}
			}
		}
	}
	return false;
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