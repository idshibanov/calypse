#include <iomanip>
#include <sstream>
#include <allegro5/allegro_primitives.h>

#include "ScreenCtl.h"
#include "Object.h"

ScreenCtl::ScreenCtl (shared_ptr<ResourceCtl> res, shared_ptr<LocalMap> map, shared_ptr<Camera> cam, 
	                  shared_ptr<Mouse> mouse, shared_ptr<AppState> stats)
	                 : frame_t(ANIMATION_TICKS) {
	_display = al_create_display(TD_DISPLAY_WIDTH, TD_DISPLAY_HEIGHT);
	al_hide_mouse_cursor(_display);

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

	_font = new SpriteText("res/arialbd.ttf", 12);

	_grass = new Sprite(0, "res/grass.png");
	_reet = new Sprite(1, "res/reet.png");
	_cursor = new Sprite(1, "res/cursor.png");

	// 44x69, 11-0-11, 6 dirs, 8 frames, T-TR, R , RD, D-LD, L, TL
	auto walk_size = _res->getObjectInfo(0).lock()->_size;
	_walk = new SpriteSheet(0, "res/f2_walk.png", 48, 8, walk_size._x, walk_size._y);

	//_current_frame = _sprites.begin();
	//_current_frame++;
	_animation_speed = 100;
	_animation_frame = 0;
}

ScreenCtl::~ScreenCtl() {
	delete _font;
	delete _grass;
	delete _reet;
	delete _cursor;
	delete _walk;

	al_destroy_display(_display);
}

bool ScreenCtl::draw() {
	bool retval = false;
	if (_render || _lastTimestamp < _state->_appTime) {
		ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);
		// map size
		unsigned rowmax = _map->getRowMax();
		unsigned colmax = _map->getColMax();

		// Map tiles
		for (unsigned row = _firstTile._y; row < _lastTile._y; row++) {
			for (unsigned col = _firstTile._x; col < _lastTile._x; col++) {
				Point coord(col, row);
				coord = (coord - _firstTile) * _tileSize + _offset;
				coord = coord.toIso() + _screenOffset;

				// loop drawing sub bitmaps must be the same parent
				//al_hold_bitmap_drawing(true);
				_grass->drawScaled(coord._x, coord._y, _tileWidth * 2, _tileHeight);
				//al_hold_bitmap_drawing(false);

				if (_state->_drawCoords) {
					string tileCoords(to_string(col) + ", " + to_string(row));
					_font->draw(tileCoords, coord._x + 24, coord._y + 8, color);
				}
			}

		}

		if (_state->_drawGrid) {
			for (int row = _firstTile._y; row < _lastTile._y; row++) {
				Point lineStart(_firstTile._x, row);
				Point lineEnd(_lastTile._x, row);

				convertMapCoords(lineStart).add(32,0);
				convertMapCoords(lineEnd).add(32, 0);

				al_draw_line(lineStart._x, lineStart._y, lineEnd._x, lineEnd._y, color, 1);
			}

			for (int col = _firstTile._x; col < _lastTile._x; col++) {
				Point lineStart(col, _firstTile._y);
				Point lineEnd(col, _lastTile._y);

				convertMapCoords(lineStart).add(32, 0);
				convertMapCoords(lineEnd).add(32, 0);

				al_draw_line(lineStart._x, lineStart._y, lineEnd._x, lineEnd._y, color, 1);
			}
		}

		shared_ptr<Actor> _actor = _map->getActor();
		auto act_info = _res->getObjectInfo(0);
		Point act_offset = act_info.lock()->_offset * _zoom;

		auto reet_info = _res->getObjectInfo(1);
		Point reet_size = reet_info.lock()->_size * _zoom;
		Point reet_offset = reet_info.lock()->_offset * _zoom;

		auto objects = _map->getObjects(_firstTile, _lastTile);

		for (auto obj : objects) {
			Point coord = obj.second->getPos();
			coord = (coord * _tileSize) / TILE_MASK + _offset - (_firstTile * _tileSize);
			coord = coord.toIso() + _screenOffset;
			if (obj.second->getType() == 0) {
				coord += act_offset;
				_walk->drawScaled(coord._x, coord._y, _actor->getSprite(), _zoom);
			} else {
				coord += reet_offset;
				_reet->drawScaled(coord._x, coord._y, reet_size._x * _zoom, reet_size._y * _zoom);
			}
			//string tileCoords(to_string(obj.second->getXPos()) + ", " + to_string(obj.second->getYPos()));
			//_font->draw(tileCoords, coord._x + 12, coord._y + 30, color);
		}

		//Point coord(_actor->getXPos(), _actor->getYPos());

		//coord = (coord * _tileSize) / TILE_MASK + _offset - (_firstTile * _tileSize);
		//coord = coord.toIso() + _screenOffset + act_offset;
		//_walk->drawScaled(coord._x, coord._y, _actor->getSprite(), _zoom);

		string timeSTR("App time: " + to_string(_state->_appTime));
		string cpsSTR("Cycles per second: " + to_string(_state->_CPS));
		string fpsSTR("Frames per second: " + to_string(_state->_FPS));
		string spdSTR("Animation speed: " + to_string(static_cast<int>(_animation_speed)) + "%");
		string frameSTR("Animation frame #" + to_string(_actor->getSprite()));
		_font->draw(timeSTR, 5, 5, color);
		_font->draw(cpsSTR, 5, 30, color);
		_font->draw(fpsSTR, 5, 55, color);
		_font->draw(spdSTR, 5, 80, color);
		_font->draw(frameSTR, 5, 105, color);

		// Almost last: mouse cursor
		_cursor->draw(_mouse->getXPos(), _mouse->getYPos());

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

		/*
		_current_frame++;
		if (_current_frame == _sprites.end()) {
			_current_frame = _sprites.begin();
			_current_frame++;
		}
		*/
		frame_t.relaunch();
	}
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
		_tileSize.set(_tileWidth, _tileHeight);
		_render = true;
	}
}

void ScreenCtl::zoomOut() {
	if (_zoom > 0.5) {
		_zoom -= 0.1;
		_tileWidth = TD_TILESIZE_X * _zoom;
		_tileHeight = TD_TILESIZE_Y * _zoom;
		_tileSize.set(_tileWidth, _tileHeight);
		_render = true;
	}
}