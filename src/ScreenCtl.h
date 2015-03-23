#pragma once
#include <vector>

#include "Main.h"
#include "AppState.h"
#include "Camera.h"
#include "Mouse.h"

#include "Map.h"
#include "ResourceCtl.h"
#include "ScreenBuffer.h"

#define TD_DISPLAY_WIDTH 800
#define TD_DISPLAY_HEIGHT 600

#define ISO_TILE_SIZE 32
#define ANIMATION_TICKS 125

class ScreenCtl {
	ALLEGRO_DISPLAY* _display;
	ScreenBuffer _buffer;
	std::shared_ptr<SpriteText> _font;

	std::shared_ptr<ResourceCtl> _res;
	std::shared_ptr<LocalMap> _map;
	std::shared_ptr<Camera> _cam;
	std::shared_ptr<Mouse> _mouse;
	std::shared_ptr<AppState> _state;

	std::shared_ptr<UIButton> _button;

	bool _render;
	int _lastTimestamp;

	unsigned _screenWidth;
	unsigned _screenHeight;
	int _tileWidth;
	int _tileHeight;
	Point _screenOffset;
	Point _offset;
	Point _firstTile;
	Point _lastTile;
	Point _tileSize;
	double _zoom;

	double _animation_speed;
	int _animation_frame;
	TaskTimer frame_t;
public:
	ScreenCtl(std::shared_ptr<ResourceCtl>, std::shared_ptr<LocalMap>, std::shared_ptr<Camera>, std::shared_ptr<Mouse>, std::shared_ptr<AppState>);
	~ScreenCtl();
	bool draw();
	void redraw(bool cameraMoved = false);
	void update();
	void updateTimers();
	std::shared_ptr<ScreenArea> processAction(const Point&);
	int XtoISO(int, int);
	int YtoISO(int, int);
	int isoXtoMap(int, int);
	int isoYtoMap(int, int);
	int convertScreenX(int, int);
	int convertScreenY(int, int);
	Point convertCoords(int, int);
	Point convertCoords(const Point& pos);
	Point& convertMapCoords(Point& coord);
	void increaseSpeed();
	void decreaseSpeed();
	void zoomIn();
	void zoomOut();
};
