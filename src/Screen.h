#pragma once
#include "Frame.h"

class Screen {
	shared_ptr<ResourceCtl> _res;

	bool _isMapScreen;

	shared_ptr<Sprite> _background;
	ALLEGRO_COLOR _backColor;

	std::vector<shared_ptr<UIFrame>> _frames;
	std::vector<shared_ptr<UIElement>> _elements;

	bool _addFrame(shared_ptr<JsonObject> entry);
	bool _addElement(shared_ptr<JsonObject> entry, UIFrame* parent = nullptr);
public:
	Screen(shared_ptr<ResourceCtl> res, const JsonObject& entry);
	virtual ~Screen();
	const std::vector<shared_ptr<UIFrame>>& getFrames() const;
	const std::vector<shared_ptr<UIElement>>& getElements() const;
	bool isMapScreen() const;
	void draw(const Point& displaySize);
};