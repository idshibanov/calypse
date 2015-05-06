#pragma once
#include <allegro5/allegro5.h>
#include "Point.h"

class ElementStyle {
public:
	bool _background;
	bool _border;
	ALLEGRO_COLOR _backColor;
	ALLEGRO_COLOR _textColor;
	ALLEGRO_COLOR _borderColor;
	Point _padding;

	ElementStyle() {
		_background = true;
		_border = true;
		_padding.set(15, 5);

		_backColor = al_map_rgb(18, 43, 82);
		_borderColor = al_map_rgb(160, 150, 93);
		_textColor = al_map_rgb(160, 150, 93);
	}
};