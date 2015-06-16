#pragma once
#include <vector>

#include "Main.h"
#include "AppState.h"
#include "Camera.h"
#include "Mouse.h"

#include "Map.h"
#include "ResourceCtl.h"
#include "ScreenBuffer.h"
#include "Screen.h"

#define TD_DISPLAY_WIDTH 800
#define TD_DISPLAY_HEIGHT 600

#define ISO_TILE_SIZE 32
#define ANIMATION_TICKS 125
#define GROUND_ITEMS_FACTOR 2

class ScreenCtl {
	ALLEGRO_DISPLAY* _display;
	ScreenBuffer _buffer;
	shared_ptr<SpriteText> _font;

	shared_ptr<ResourceCtl> _res;
	shared_ptr<LocalMap> _map;
	shared_ptr<Camera> _cam;
	shared_ptr<Mouse> _mouse;
	shared_ptr<AppState> _state;

	std::vector<shared_ptr<Screen>> _screens;
	std::vector<shared_ptr<Screen>>::iterator _curScreen;
	std::map<std::string, int> _screenLookup;

	std::vector<shared_ptr<UIFrame>> _frames;
	std::vector<shared_ptr<UIButton>> _options;
	shared_ptr<CarouselMenu> _menu;

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
	ScreenCtl(shared_ptr<ResourceCtl>, shared_ptr<LocalMap>, shared_ptr<Camera>, shared_ptr<Mouse>, shared_ptr<AppState>);
	~ScreenCtl();
	void loadScreen();
	bool switchScreen(const std::string& name);
	bool draw();
	void redraw(bool cameraMoved = false);
	void update();
	void updateTimers();
	void drawSelectedObject(shared_ptr<ObjectInfo> obj);
	shared_ptr<ScreenArea> processAction(const Point&);
	void displayOptions(Point, const shared_ptr<ObjectActionArea>);
	void hideOptions();
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
	void toggleInfoScreen();
	void toggleInventory();
};
