#include <iomanip>
#include <sstream>

#include "ScreenCtl.h"

ScreenCtl::ScreenCtl() 
    : _grass(0, "res/grass.png"),
	frame_t(ANIMATION_TICKS) {
	_display = al_create_display(800, 600);

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

	_font = al_load_font("res/arialbd.ttf", 12, 2);

	_current_frame = _sprites.begin();
	_current_frame++;
	_animation_speed = 100;
	_animation_frame = 0;
}

ScreenCtl::~ScreenCtl() {
	for (auto it = _sprites.begin(); it != _sprites.end(); ++it) {
		al_destroy_bitmap(*it);
	}

	if (_font)
		al_destroy_font(_font);

	al_destroy_display(_display);
}

void ScreenCtl::draw(AppStats& stat) {
	int xOffset = 368;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			_grass.draw(xOffset + XtoISO(i * 32, j * 32), YtoISO(i * 32, j * 32));
		}
	}
	al_draw_bitmap(*_current_frame, 0, 0, 0);

	string timeSTR("Game time: " + to_string(static_cast<long long>(stat._gameTime)));
	string cpsSTR("Cycles per second: " + to_string(static_cast<long long>(stat._CPS)));
	string fpsSTR("Frames per second: " + to_string(static_cast<long long>(stat._FPS)));
	string spdSTR("Animation speed: " + to_string(static_cast<long long>(_animation_speed)) + "%");
	string frameSTR("Animation frame #" + to_string(static_cast<long long>(_animation_frame)));
	al_draw_text(_font, al_map_rgb(255, 255, 255), 5, 5, 0, timeSTR.c_str());
	al_draw_text(_font, al_map_rgb(255, 255, 255), 5, 30, 0, cpsSTR.c_str());
	al_draw_text(_font, al_map_rgb(255, 255, 255), 5, 55, 0, fpsSTR.c_str());
	al_draw_text(_font, al_map_rgb(255, 255, 255), 5, 80, 0, spdSTR.c_str());
	al_draw_text(_font, al_map_rgb(255, 255, 255), 5, 105, 0, frameSTR.c_str());
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