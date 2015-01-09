#pragma once
#include <allegro5\allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "main.h"
#include "ScreenCtl.h"

#define CLOCK_SPEED 1000
#define ANIMATION_TICKS 50

enum KEYS{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SHIFT,
	CTRL,
	ALT
};

class AppCtl {
	ALLEGRO_DISPLAY* _display;
	ALLEGRO_EVENT_QUEUE* _eventQueue;
	ALLEGRO_TIMER* _timer;

	bool _isRunning;
	bool _render;
	float _gameTime;
	int _render_frames;
	int _animation_frame;
	int _cycles;
	int _CPS;
	int _FPS;
	bool* _keys;

	vector<ALLEGRO_BITMAP*> _sprites;
	ALLEGRO_FONT* _font;

	AppCtl(const AppCtl&);
	AppCtl& operator=(const AppCtl&);
public:
	AppCtl();
	~AppCtl();
	void render();
	void update();
	void controlLoop();
};