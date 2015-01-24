#include <iomanip>
#include <sstream>

#include "ScreenCtl.h"
#include "Object.h"

ScreenCtl::ScreenCtl(shared_ptr<LocalMap> map, shared_ptr<Camera> cam, shared_ptr<Mouse> mouse, shared_ptr<AppStats> stats)
	: frame_t(ANIMATION_TICKS) {
	_display = al_create_display(TD_DISPLAY_WIDTH, TD_DISPLAY_HEIGHT);
	al_hide_mouse_cursor(_display);

	_screenWidth = TD_DISPLAY_WIDTH;
	_screenHeight = TD_DISPLAY_HEIGHT;
	_tileWidth = TD_TILESIZE_X;
	_tileHeight = TD_TILESIZE_Y;

	_map = map;
	_cam = cam;
	_mouse = mouse;
	_stats = stats;

	_render = true;
	_lastTimestamp = _stats->_gameTime;

	_renderX = 0;
	_renderY = 0;
	_tileCol = 0;
	_tileRow = 0;
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
	_walk = new SpriteSheet(0, "res/f2_walk.png", 48, 8, 44, 69);

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
	if (_render || _lastTimestamp < _stats->_gameTime) {
		ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);
		// map size
		unsigned rowmax = _map->getRowMax();
		unsigned colmax = _map->getColMax();
		unsigned maxTileCol = 0, maxTileRow = 0;


		maxTileCol = _tileCol + ((-_offsetX + _screenWidth + _maxOffset) / _tileWidth);
		if (colmax < maxTileCol) maxTileCol = colmax;
		maxTileRow = _tileRow + ((-_offsetY + _screenHeight + _maxOffset) / _tileHeight);
		if (rowmax < maxTileRow) maxTileRow = rowmax;

		// Map tiles
		for (unsigned row = _tileRow; row < maxTileRow; row++) {
			for (unsigned col = _tileCol; col < maxTileCol; col++) {
				int x_coord = _renderX + XtoISO(_offsetX + (col - _tileCol) * _tileWidth, _offsetY + (row - _tileRow) * _tileHeight);
				int y_coord = _renderY + YtoISO(_offsetX + (col - _tileCol) * _tileWidth, _offsetY + (row - _tileRow) * _tileHeight);
				// loop drawing sub bitmaps must be the same parent
				//al_hold_bitmap_drawing(true);
				_grass->drawScaled(x_coord, y_coord, _tileWidth * 2, _tileHeight);
				//al_hold_bitmap_drawing(false);

				//string tileCoords(to_string(col) + ", " + to_string(row));
				//_font->draw(tileCoords, x_coord + 24, y_coord + 8, color);
			}
		}

		for (unsigned row = _tileRow; row < maxTileRow; row++) {
			for (unsigned col = _tileCol; col < maxTileCol; col++) {
				if (_map->getTileType(row * rowmax + col) == 10) {
					int x_coord = _renderX + XtoISO(_offsetX + (col - _tileCol) * _tileWidth, _offsetY + (row - _tileRow) * _tileHeight);
					int y_coord = _renderY - (60*_zoom) + YtoISO(_offsetX + (col - _tileCol) * _tileWidth, _offsetY + (row - _tileRow) * _tileHeight);
					//_reet->drawScaled(x_coord, y_coord, 64 * _zoom, 92 * _zoom);
				}
			}
		}

		shared_ptr<Actor> _actor = _map->getActor();
		int x_coord = _renderX + (10 * _zoom) + XtoISO(_offsetX + _actor->getXPos() * _tileWidth, _offsetY + _actor->getYPos() * _tileHeight);
		int y_coord = _renderY - (45 * _zoom) + YtoISO(_offsetX + _actor->getXPos() * _tileWidth, _offsetY + _actor->getYPos() * _tileHeight);
		//_current_frame->drawScaled(x_coord, y_coord, _zoom / 2);
		_walk->drawScaled(x_coord, y_coord, _actor->getSprite(), _zoom);

		string timeSTR("Game time: " + to_string(static_cast<long long>(_stats->_gameTime)));
		string cpsSTR("Cycles per second: " + to_string(static_cast<long long>(_stats->_CPS)));
		string fpsSTR("Frames per second: " + to_string(static_cast<long long>(_stats->_FPS)));
		string spdSTR("Animation speed: " + to_string(static_cast<long long>(_animation_speed)) + "%");
		string frameSTR("Animation frame #" + to_string(static_cast<long long>(_animation_frame)));
		_font->draw(timeSTR, 5, 5, color);
		_font->draw(cpsSTR, 5, 30, color);
		_font->draw(fpsSTR, 5, 55, color);
		_font->draw(spdSTR, 5, 80, color);
		_font->draw(frameSTR, 5, 105, color);

		// Almost last: mouse cursor
		_cursor->draw(_mouse->getXPos(), _mouse->getYPos());

		_lastTimestamp = _stats->_gameTime;
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
	unsigned camX = _cam->getXPos();
	unsigned camY = _cam->getYPos();
	// first tile to render - coords and array id
	int firstTileX = 0, firstTileY = 0;
	_tileCol = 0;
	_tileRow = 0;
	// display coords to start rendering

	_offsetX = 0, _offsetY = 0;

	_renderX = _screenWidth / 2 - _tileWidth;
	_offsetX -= camX;
	_renderY = _screenHeight / 2;
	_offsetY -= camY;

	_maxOffset = (_screenWidth / 2 + _screenHeight) / 2;
	if (camX > _maxOffset + _tileWidth) {
		_tileCol = (camX - _maxOffset) / _tileWidth;
		_offsetX += _tileCol * _tileWidth;
	}
	if (camY > _maxOffset + _tileHeight) {
		_tileRow = (camY - _maxOffset) / _tileHeight;
		_offsetY += _tileRow * _tileHeight;
	}

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
	int trueX = isoXtoMap(x - _renderX - _tileWidth, y - _renderY) - _offsetX;

	return trueX / _tileWidth + _tileCol;
}

int ScreenCtl::convertScreenY(int x, int y) {
	int trueY = isoYtoMap(x - _renderX - _tileWidth, y - _renderY) - _offsetY;

	return trueY / _tileHeight + _tileRow;
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
		_render = true;
	}
}

void ScreenCtl::zoomOut() {
	if (_zoom > 0.5) {
		_zoom -= 0.1;
		_tileWidth = TD_TILESIZE_X * _zoom;
		_tileHeight = TD_TILESIZE_Y * _zoom;
		_render = true;
	}
}