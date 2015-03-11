#include <iomanip>
#include <sstream>
#include <allegro5/allegro_primitives.h>

#include "ScreenCtl.h"
#include "Object.h"

ScreenCtl::ScreenCtl (shared_ptr<ResourceCtl> res, shared_ptr<LocalMap> map, shared_ptr<Camera> cam, 
	                  shared_ptr<Mouse> mouse, shared_ptr<AppState> stats)
	                 : frame_t(ANIMATION_TICKS) {
	//al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	//al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_REQUIRE);

	_display = al_create_display(TD_DISPLAY_WIDTH, TD_DISPLAY_HEIGHT);
	al_hide_mouse_cursor(_display);

	// load sprites only after setting up display
	res->loadSprites();

	_screenWidth = TD_DISPLAY_WIDTH;
	_screenHeight = TD_DISPLAY_HEIGHT;
	_tileWidth = TD_TILESIZE_X;
	_tileHeight = TD_TILESIZE_Y;
	_tileSize.set(_tileWidth, _tileHeight);

	_res = res;
	_map = map;
	_cam = cam;
	_mouse = mouse;
	_state = stats;

	_render = true;
	_lastTimestamp = _state->_appTime;

	_zoom = 1.0;

	_font = new SpriteText("res/arialbd.ttf", 12);

	/*
	_sprites.reserve(10*20);
	for (int dir = 0; dir < 1; dir++) {
		for (int i = 0; i < 21; i++) {
			ostringstream ss;
			ss << setw(4) << setfill('0') << i;
			string res_path("res/char/run_" + to_string(dir) + "_" + ss.str() + ".png");
			_sprites.emplace_back(dir * 20 + i, res_path.c_str(), 298, 235, 125, 185);
		}
	}
	*/


	_animation_speed = 100;
	_animation_frame = 0;
}

ScreenCtl::~ScreenCtl() {
	delete _font;

	al_destroy_display(_display);
}

bool ScreenCtl::draw() {
	bool retval = false;
	if (_render || _lastTimestamp < _state->_appTime) {
		_buffer.reset();
		ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);
		// map size
		unsigned rowmax = _map->getRowMax();
		unsigned colmax = _map->getColMax();
		Point isoTileSize(_tileSize._x * 2, _tileSize._y);

		// Map tiles
		auto grass = _res->getSprite(1).get();
		for (int row = _firstTile._y; row < _lastTile._y; row++) {
			for (int col = _firstTile._x; col < _lastTile._x; col++) {
				Point coord(col, row);
				coord = (coord - _firstTile) * _tileSize + _offset;
				coord = coord.toIso() + _screenOffset;

				// loop drawing sub bitmaps must be the same parent
				//al_hold_bitmap_drawing(true);
				grass->drawScaled(coord, isoTileSize);
				//al_hold_bitmap_drawing(false);

				if (_state->_drawCoords) {
					string tileCoords(to_string(col) + ", " + to_string(row));
					_font->draw(tileCoords, coord.add(24,8), color);
				}
			}
		}

		if (_state->_drawGrid) {
			for (int row = _firstTile._y; row < _lastTile._y; row++) {
				Point lineStart(_firstTile._x, row);
				Point lineEnd(_lastTile._x, row);

				convertMapCoords(lineStart).add(32 * _zoom, 0);
				convertMapCoords(lineEnd).add(32 * _zoom, 0);

				al_draw_line(lineStart._x, lineStart._y, lineEnd._x, lineEnd._y, color, 1);
			}

			for (int col = _firstTile._x; col < _lastTile._x; col++) {
				Point lineStart(col, _firstTile._y);
				Point lineEnd(col, _lastTile._y);

				convertMapCoords(lineStart).add(32 * _zoom, 0);
				convertMapCoords(lineEnd).add(32 * _zoom, 0);

				al_draw_line(lineStart._x, lineStart._y, lineEnd._x, lineEnd._y, color, 1);
			}
		}

		shared_ptr<Actor> _actor = _map->getActor();
		auto renderedObjects = _map->getObjects(_firstTile, _lastTile);

		for (auto obj : renderedObjects) {
			Point coord = obj.second->getPos();
			coord = (coord * _tileSize) / TILE_MASK + _offset - (_firstTile * _tileSize);
			coord = coord.toIso() + _screenOffset;

			int type = obj.second->getType();
			auto objInfo = _res->getObjectInfo(type).lock();

			if (objInfo != nullptr) {
				Point objSize = objInfo->_size * _zoom;
				coord += objInfo->_offset * _zoom;

				Sprite* obj_spr = _res->getSprite(objInfo->_spriteID).get();
				SpriteSheet* objSprite = dynamic_cast<SpriteSheet*>(obj_spr);

				objSprite->drawScaled(coord, obj.second->getSprite(), _zoom);

				_buffer.setElement(ScreenArea(coord, objSize, obj.second, objSprite));
				//al_draw_rectangle(coord._x, coord._y, coord._x + objSize._x, coord._y + objSize._y,
					//al_map_rgb(255, 100, 100), 1.0);

				//string tileCoords(to_string(obj.second->getXPos()) + ", " + to_string(obj.second->getYPos()));
				//_font->draw(tileCoords, coord.add(12, 30), color);
				//_font->draw(to_string(obj.first), coord.add(12, 30), color);
				_font->draw(to_string(obj.second->getID()), coord.add(12, 30), color);
			}
		}

		string timeSTR("App time: " + to_string(_state->_appTime));
		string cpsSTR("Cycles per second: " + to_string(_state->_CPS));
		string fpsSTR("Frames per second: " + to_string(_state->_FPS));
		string spdSTR("Animation speed: " + to_string(static_cast<int>(_animation_speed)) + "%");
		string frameSTR("Animation frame #" + to_string(_actor->getSprite()));
		_font->draw(timeSTR, Point(5, 5), color);
		_font->draw(cpsSTR, Point(5, 30), color);
		_font->draw(fpsSTR, Point(5, 55), color);
		_font->draw(spdSTR, Point(5, 80), color);
		_font->draw(frameSTR, Point(5, 105), color);

		// Almost last: mouse cursor
		_res->getSprite(0)->draw(_mouse->getPos());

		_lastTimestamp = _state->_appTime;
		_render = false;
		retval = true;
	}

	return retval;
}


void ScreenCtl::redraw(bool cameraMoved) {
	if (cameraMoved) update();

	_render = true;
}


void ScreenCtl::update() {
	// camera position
	Point camPos = _cam->getPos();
	// map rendering offset, based on camera position
	_offset.set(0, 0);

	// offset after which we shift the tiles
	int maxOffset = (_screenWidth / 2 + _screenHeight) / 2;

	_screenOffset.set(_screenWidth / 2 - _tileWidth, _screenHeight / 2);
	_tileSize.set(_tileWidth, _tileHeight);
	_offset -= camPos;

	if (camPos._x > maxOffset + _tileSize._x) {
		_firstTile._x = (camPos._x - maxOffset) / _tileSize._x;
	}
	if (camPos._y > maxOffset + _tileSize._y) {
		_firstTile._y = (camPos._y - maxOffset) / _tileSize._y;
	}
	_offset += (_firstTile * _tileSize);

	unsigned rowmax = _map->getRowMax();
	unsigned colmax = _map->getColMax();

	// calculate last tile to render, based on current offset
	_lastTile = _offset;
	_lastTile.inv().add(_screenWidth + maxOffset, _screenHeight + maxOffset);
	_lastTile = _firstTile + (_lastTile / _tileSize);
	if (colmax < _lastTile._x) _lastTile._x = colmax;
	if (rowmax < _lastTile._y) _lastTile._y = rowmax;

	_render = true;
}


void ScreenCtl::updateTimers() {
	if (frame_t.check()) {
		_render = true;
		_animation_frame++;
		if (_animation_frame > 47) _animation_frame = 0;
		frame_t.relaunch();
	}
}

shared_ptr<MapObject> ScreenCtl::processAction(const Point& pos) {
	return _buffer.getElement(pos);
}

int ScreenCtl::XtoISO(int x, int y) {
	return x - y;
}

int ScreenCtl::YtoISO(int x, int y) {
	return (x + y) / 2;
}

int ScreenCtl::isoXtoMap(int x, int y) {
	return (2 * y + x) / 2;
}

int ScreenCtl::isoYtoMap(int x, int y) {
	return (2 * y - x) / 2;
}

int ScreenCtl::convertScreenX(int x, int y) {
	return convertCoords(x, y)._x;
}

int ScreenCtl::convertScreenY(int x, int y) {
	return convertCoords(x, y)._y;
}

Point ScreenCtl::convertCoords(int x, int y) {
	Point coord(x - _tileSize._x, y);
	coord -= _screenOffset;
	return ((coord.toMap() - _offset) * TILE_MASK) / _tileSize + _firstTile * TILE_MASK;
}

Point ScreenCtl::convertCoords(const Point& pos) {
	return convertCoords(pos._x, pos._y);
}

Point& ScreenCtl::convertMapCoords(Point& coord) {
	coord = (coord - _firstTile) * _tileSize + _offset;
	coord = coord.toIso() + _screenOffset;
	return coord;
}

void ScreenCtl::increaseSpeed() {
	_animation_speed += 10;
	frame_t.adjust(ANIMATION_TICKS / _animation_speed * 100);
}

void ScreenCtl::decreaseSpeed() {
	if (_animation_speed > 10) {
		_animation_speed -= 10;
		frame_t.adjust(ANIMATION_TICKS / _animation_speed * 100);
	}
}

void ScreenCtl::zoomIn() {
	if (_zoom < 2.0) {
		_zoom += 0.1;
		_tileWidth = TD_TILESIZE_X * _zoom;
		_tileHeight = TD_TILESIZE_Y * _zoom;
		update();
	}
}

void ScreenCtl::zoomOut() {
	if (_zoom > 0.5) {
		_zoom -= 0.1;
		_tileWidth = TD_TILESIZE_X * _zoom;
		_tileHeight = TD_TILESIZE_Y * _zoom;
		update();
	}
}