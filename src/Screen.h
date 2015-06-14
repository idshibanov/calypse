#pragma once
#include "Frame.h"

class Screen {
	std::vector<shared_ptr<UIFrame>> _frames;
	std::vector<shared_ptr<UIButton>> _options;
	shared_ptr<CarouselMenu> _menu;
public:
	Screen();
	virtual ~Screen();
	void draw();
};