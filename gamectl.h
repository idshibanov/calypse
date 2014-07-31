#pragma once
#include <allegro5\allegro5.h>
#include "action.h"
#include "camera.h"
#include "cursor.h"
#include "map.h"
#include "main.h"
#include "mouse.h"
#include "screenctl.h"
#include "pathfinder.h"

enum KEYS{
    UP, 
    DOWN, 
    LEFT,
    RIGHT,
    SHIFT,
    CTRL,
    ALT
};

class GameCtl{
    ALLEGRO_DISPLAY* _display;
	ALLEGRO_EVENT_QUEUE* _eventQueue;
	ALLEGRO_TIMER* _timer;
    
    ScreenCtl* _screen;
    shared_ptr<GameMap> _map;
    shared_ptr<AStarSearch> _pFinder;
    TCamera* _cam;
    TCursor* _cursor;
    TMouse* _mouse;

    bool _isRunning;
	bool _render;
	float _gameTime;
	int _frames;
	int _gameFPS;
    bool* _keys;
    int _actionID;

    GameCtl(const GameCtl&);
    GameCtl& operator=(const GameCtl&);
public:
    GameCtl();
    ~GameCtl();
    void gameRender();
    void gameInit();
    void gameUpdate();
    void gameRun();
    void processScreenAction(TMouse& mouse);
    void processUIAction(int absX, int absY, int button);
};