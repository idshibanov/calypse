#pragma once
#include "Frame.h"

class Screen {
	bool _isMapScreen;

	shared_ptr<Sprite> _background;
	ALLEGRO_COLOR _backColor;

	std::vector<shared_ptr<UIFrame>> _frames;
	std::vector<shared_ptr<UIElement>> _elements;

	bool _addFrame(shared_ptr<JsonObject> entry);
public:
	Screen(const JsonObject& entry);
	virtual ~Screen();
	const std::vector<shared_ptr<UIFrame>>& getFrames() const;
	const std::vector<shared_ptr<UIElement>>& getElements() const;
	bool isMapScreen() const;
	void draw(const Point& displaySize);
};