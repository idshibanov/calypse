#include <iomanip>
#include <sstream>

#include "ScreenCtl.h"

ScreenCtl::ScreenCtl(shared_ptr<LocalMap> map, shared_ptr<Camera> cam) : frame_t(ANIMATION_TICKS) {
	_display = al_create_display(TD_DISPLAY_WIDTH, TD_DISPLAY_HEIGHT);

	_screenWidth = TD_DISPLAY_WIDTH;
	_screenHeight = TD_DISPLAY_HEIGHT;
	_tileWidth = TD_TILESIZE_X;
	_tileHeight = TD_TILESIZE_Y;

	_map = map;
	_cam = cam;
	_renderX = 0;
	_renderY = 0;
	_tileCol = 0;
	_tileRow = 0;
	_zoom = 1.0;

	for (int i = 0; i < 21; i++) {
		ostringstream ss;
		ss << setw(4) << setfill('0') << i;
		string res_path("res/run_2_" + ss.str() + ".png");

		ALLEGRO_BITMAP* _texture = al_load_bitmap(res_path.c_str());

		if (!_texture) {
			printf("failed to load sprite %d! %s\n", i, res_path.c_str());
		}
		else {
			_sprites.push_back(_texture);
		}
	}

	_font = new SpriteText("res/arialbd.ttf", 12);

	_grass = new Sprite(0, "res/grass.png");

	_current_frame = _sprites.begin();
	_current_frame++;
	_animation_speed = 100;
	_animation_frame = 0;
}

ScreenCtl::~ScreenCtl() {
	for (auto it = _sprites.begin(); it != _sprites.end(); ++it) {
		al_destroy_bitmap(*it);
	}

	delete _font;
	delete _grass;

	al_destroy_display(_display);
}

void ScreenCtl::draw(AppStats& stat) {
	// TODO: calculate offsets only when camera moves, not on every draw

	// map size
	unsigned rowmax = _map->getRowMax();
	unsigned colmax = _map->getColMax();
	// camera position
	unsigned camX = 0;// _cam->getXPos();
	unsigned camY = 640;// _cam->getYPos();
	// first tile to render - coords and array id
	int firstTileX = 0, firstTileY = 0;
	unsigned maxTileCol = 0, maxTileRow = 0;
	_tileCol = 0;
	_tileRow = 0;
	// display coords to start rendering
	_renderX = _screenWidth / 2 - _tileWidth;
	_renderY = 0;


	if (camX < _screenWidth / 2){
		// adjust render X position (left border)
		_renderX = _screenWidth / 2 - camX;
	}
	else {
		// find out first tile to render
		firstTileX = camX - _screenWidth / 2;
		_tileCol = firstTileX / _tileWidth;

		// adjust render X position if cam X coord is uneven
		if (firstTileX % _tileWidth){
			_renderX = -(firstTileX % _tileWidth);
		}
	}

	if (camY < _screenHeight / 2){
		// adjust render Y position (top border)
		_renderY = _screenHeight / 2 - camY;
	}
	else {
		// find out first tile to render
		firstTileY = camY - _screenHeight / 2;
		_tileRow = firstTileY / _tileHeight;

		// adjust render Y position if cam Y coord is uneven
		if (firstTileY % _tileHeight){
			_renderY = -(firstTileY % _tileHeight);
		}
	}


	// +3 tiles to handle the camera movement
	maxTileCol = _tileCol + ((_renderX + _screenWidth) / _tileWidth) + 3;
	if (colmax < maxTileCol) maxTileCol = colmax;
	maxTileRow = _tileRow + ((_renderY + TD_DISPLAY_HEIGHT) / _tileHeight) + 3;
	if (rowmax < maxTileRow) maxTileRow = rowmax;

	// Map tiles
	for (unsigned row = _tileRow; row < maxTileRow; row++){
		for (unsigned col = _tileCol; col < maxTileCol; col++){
			int x_coord = XtoISO((col - _tileCol) * _tileWidth, (row - _tileRow) * _tileHeight);
			int y_coord = YtoISO((col - _tileCol) * _tileWidth, (row - _tileRow) * _tileHeight);
			_grass->drawScaled(_renderX + x_coord,
				_renderY + y_coord,
				64, 32);
		}
	}
	/*
	int xOffset = al_get_display_width(_display) / 2 - ISO_TILE_SIZE;
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			_grass->draw(xOffset + XtoISO(i * 32, j * 32), YtoISO(i * 32, j * 32));
		}
	}
	*/
	al_draw_bitmap(*_current_frame, 0, 0, 0);

	string timeSTR("Game time: " + to_string(static_cast<long long>(stat._gameTime)));
	string cpsSTR("Cycles per second: " + to_string(static_cast<long long>(stat._CPS)));
	string fpsSTR("Frames per second: " + to_string(static_cast<long long>(stat._FPS)));
	string spdSTR("Animation speed: " + to_string(static_cast<long long>(_animation_speed)) + "%");
	string frameSTR("Animation frame #" + to_string(static_cast<long long>(_animation_frame)));
	ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);
	_font->draw(timeSTR, 5, 5, color);
	_font->draw(cpsSTR, 5, 30, color);
	_font->draw(fpsSTR, 5, 55, color);
	_font->draw(spdSTR, 5, 80, color);
	_font->draw(frameSTR, 5, 105, color);
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