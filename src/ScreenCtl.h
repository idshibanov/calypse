#pragma once
#include <vector>
#include "Main.h"
#include "Sprite.h"
#include "Timer.h"
#include "Camera.h"
#include "Map.h"

#define TD_DISPLAY_WIDTH 800
#define TD_DISPLAY_HEIGHT 600

#define ISO_TILE_SIZE 32
#define ANIMATION_TICKS 50

struct AppStats {
	int _gameTime;
	int _CPS;
	int _FPS;
};

class ScreenCtl {
	ALLEGRO_DISPLAY* _display;
	vector<RawSprite> _sprites;
	vector<RawSprite>::iterator _current_frame;
	SpriteText* _font;
	Sprite* _grass;
	Sprite* _reet;

	shared_ptr<LocalMap> _map;
	shared_ptr<Camera> _cam;
	unsigned _screenWidth;
	unsigned _screenHeight;
	int _tileWidth;
	int _tileHeight;
	int _tileCol;
	int _tileRow;
	int _renderX;
	int _renderY;
	double _zoom;

	double _animation_speed;
	int _animation_frame;
	TaskTimer frame_t;
public:
	ScreenCtl(shared_ptr<LocalMap> map, shared_ptr<Camera> cam);
	~ScreenCtl();
	void draw(AppStats& stat);
	void updateTimers();
	int XtoISO(int, int);
	int YtoISO(int, int);
	void increaseSpeed();
	void decreaseSpeed();
	void zoomIn();
	void zoomOut();
};
