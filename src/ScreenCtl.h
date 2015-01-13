#pragma once
#include <vector>
#include "Main.h"
#include "Sprite.h"
#include "Timer.h"

#define ISO_TILE_SIZE 32
#define ANIMATION_TICKS 50

struct AppStats {
	int _gameTime;
	int _CPS;
	int _FPS;
};

class ScreenCtl {
	ALLEGRO_DISPLAY* _display;
	vector<ALLEGRO_BITMAP*> _sprites;
	vector<ALLEGRO_BITMAP*>::iterator _current_frame;
	ALLEGRO_FONT* _font;
	Sprite* _grass;

	double _animation_speed;
	int _animation_frame;
	TaskTimer frame_t;
public:
	ScreenCtl();
	~ScreenCtl();
	void draw(AppStats& stat);
	void updateTimers();
	int XtoISO(int, int);
	int YtoISO(int, int);
	void increaseSpeed();
	void decreaseSpeed();
};
