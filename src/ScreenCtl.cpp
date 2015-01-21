#include <iomanip>
#include <sstream>

#include "ScreenCtl.h"

ScreenCtl::ScreenCtl(shared_ptr<LocalMap> map, shared_ptr<Camera> cam, shared_ptr<AppStats> stats) 
	: frame_t(ANIMATION_TICKS) {
	_display = al_create_display(TD_DISPLAY_WIDTH, TD_DISPLAY_HEIGHT);

	_screenWidth = TD_DISPLAY_WIDTH;
	_screenHeight = TD_DISPLAY_HEIGHT;
	_tileWidth = TD_TILESIZE_X;
	_tileHeight = TD_TILESIZE_Y;

	_map = map;
	_cam = cam;
	_stats = stats;

	_render = true;
	_lastTimestamp = _stats->_gameTime;

	_renderX = 0;
	_renderY = 0;
	_tileCol = 0;
	_tileRow = 0;
	_zoom = 1.0;

	_sprites.reserve(8*20);
	for (int dir = 0; dir < 1; dir++) {
		for (int i = 0; i < 21; i++) {
			ostringstream ss;
			ss << setw(4) << setfill('0') << i;
			string res_path("res/char/run_" + to_string(dir) + "_" + ss.str() + ".png");
			_sprites.emplace_back(dir * 20 + i, res_path.c_str(), 298, 248, 125, 175);
		}
	}

	_font = new SpriteText("res/arialbd.ttf", 12);

	_grass = new Sprite(0, "res/grass.png");
	_reet = new Sprite(1, "res/reet.png");

	_current_frame = _sprites.begin();
	_current_frame++;
	_animation_speed = 100;
	_animation_frame = 0;
}

ScreenCtl::~ScreenCtl() {
	delete _font;
	delete _grass;
	delete _reet;

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
					_reet->drawScaled(x_coord, y_coord, 64 * _zoom, 92 * _zoom);
				}
			}
		}

		//_current_frame->drawScaled(100, 100, _zoom/2);

		//string timeSTR("Game time: " + to_string(static_cast<long long>(_stats->_gameTime)));
		//string cpsSTR("Cycles per second: " + to_string(static_cast<long long>(_stats->_CPS)));
		//string fpsSTR("Frames per second: " + to_string(static_cast<long long>(_stats->_FPS)));
		//string spdSTR("Animation speed: " + to_string(static_cast<long long>(_animation_speed)) + "%");
		//string frameSTR("Animation frame #" + to_string(static_cast<long long>(_animation_frame)));
		string timeSTR("_tileCol: " + to_string(_tileCol));
		string cpsSTR("_tileRow: " + to_string(_tileRow));
		string fpsSTR("_offsetX: " + to_string(_offsetX));
		string spdSTR("_offsetY: " + to_string(_offsetY));
		string frameSTR("_zoom: " + to_string(_zoom));
		_font->draw(timeSTR, 5, 5, color);
		_font->draw(cpsSTR, 5, 30, color);
		_font->draw(fpsSTR, 5, 55, color);
		_font->draw(spdSTR, 5, 80, color);
		_font->draw(frameSTR, 5, 105, color);

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
		_animation_frame++;
		if (_animation_frame > 20) _animation_frame = 0;

		_current_frame++;
		if (_current_frame == _sprites.end()) {
			_current_frame = _sprites.begin();
			_current_frame++;
		}
		frame_t.relaunch();
	}
}

int ScreenCtl::XtoISO(int x, int y) {
	return x - y;
}

int ScreenCtl::YtoISO(int x, int y) {
	return (x + y) / 2;
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