#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "ScreenCtl.h"

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

	std::shared_ptr<ResourceCtl> _res;
	std::shared_ptr<LocalMap> _map;
	std::shared_ptr<AStarSearch> _pFinder;
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<Mouse> _mouse;
	std::shared_ptr<AppState> _state;

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