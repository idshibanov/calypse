#pragma once
#include <vector>

#include "Main.h"
#include "AppState.h"
#include "Camera.h"
#include "Mouse.h"

#include "Map.h"
#include "ResourceCtl.h"

#define TD_DISPLAY_WIDTH 800
#define TD_DISPLAY_HEIGHT 600

#define ISO_TILE_SIZE 32
#define ANIMATION_TICKS 125

class ScreenCtl {
	ALLEGRO_DISPLAY* _display;
	vector<RawSprite> _sprites;
	vector<RawSprite>::iterator _current_frame;
	SpriteText* _font;
	Sprite* _grass;
	Sprite* _reet;
	Sprite* _cursor;
	SpriteSheet* _walk;

	shared_ptr<ResourceCtl> _res;
	shared_ptr<LocalMap> _map;
	shared_ptr<Camera> _cam;
	shared_ptr<Mouse> _mouse;
	shared_ptr<AppState> _state;

	bool _render;
	int _lastTimestamp;
	map<int, shared_ptr<MapObject>> _renderedObjects;

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
	ScreenCtl(shared_ptr<ResourceCtl>, shared_ptr<LocalMap>, shared_ptr<Camera>, shared_ptr<Mouse>, shared_ptr<AppState>);
	~ScreenCtl();
	bool draw();
	void redraw(bool cameraMoved = false);
	void update();
	void updateTimers();
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
