#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "ScreenCtl.h"
#include "EventService.h"
#include "Keyboard.h"

#define CLOCK_SPEED 1000

class AppCtl {
	ScreenCtl* _screen;
	ALLEGRO_EVENT_QUEUE* _eventQueue;
	ALLEGRO_TIMER* _timer;

	shared_ptr<ResourceCtl> _res;
	shared_ptr<LocalMap> _map;
	shared_ptr<AStarSearch> _pFinder;
	shared_ptr<EventService> _events;
	shared_ptr<Camera> _camera;
	shared_ptr<Mouse> _mouse;
	shared_ptr<AppState> _state;

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
	void processMouseAction();
	void render();
	void update();
	void controlLoop();
};