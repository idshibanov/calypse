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
	shared_ptr<AppState> _state;
	shared_ptr<ConfigCtl> _config;
	shared_ptr<ResourceCtl> _res;

	ALLEGRO_DISPLAY* _display;
	ALLEGRO_EVENT_QUEUE* _eventQueue;
	ALLEGRO_TIMER* _timer;

	shared_ptr<Mouse> _mouse;
	shared_ptr<LocalMap> _map;
	shared_ptr<AStarSearch> _pFinder;
	shared_ptr<EventService> _events;
	shared_ptr<Camera> _camera;
	
	std::map<int, shared_ptr<Screen>> _screenMap;
	std::map<std::string, int> _screenLookup;

	bool _isRunning;
	bool _mapIsActive;
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
	void loadInstance();
	void offload();
	void render();
	void update();
	void controlLoop();
	bool switchScreen(const std::string& name);
	void processMouseAction();
	void processLeftClick(const Point& clickPos);
	void processRightClick(const Point& clickPos);
	void processMapElement();
	void processUIElement(shared_ptr<UIElement> elem);
	void selectAction(int actID);
	void selectAction(const char* command);
	void selectItem(shared_ptr<Item> it);
};