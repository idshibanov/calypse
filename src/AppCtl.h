#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "Main.h"
#include "ScreenCtl.h"
#include "Camera.h"
#include "Map.h"
#include "Pathfinder.h"

#define CLOCK_SPEED 1000

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
	ScreenCtl* _screen;
	ALLEGRO_EVENT_QUEUE* _eventQueue;
	ALLEGRO_TIMER* _timer;

	shared_ptr<LocalMap> _map;
	shared_ptr<AStarSearch> _pFinder;
	shared_ptr<Camera> _camera;

	bool _isRunning;
	bool _render;
	float _gameTime;
	int _render_frames;
	int _animation_frame;
	int _cycles;
	int _CPS;
	int _FPS;
	bool* _keys;

	AppCtl(const AppCtl&);
	AppCtl& operator=(const AppCtl&);
public:
	AppCtl();
	~AppCtl();
	void render();
	void update();
	void controlLoop();
};