#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

#include "AppCtl.h"
#include "timer.h"

int XtoISO(int x, int y) {
	return x - y;
}

int YtoISO(int x, int y) {
	return (x + y) / 2;
}

AppCtl::AppCtl() {
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

	_eventQueue = al_create_event_queue();
	_timer = al_create_timer(1.0 / CLOCK_SPEED);
	al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));
	al_register_event_source(_eventQueue, al_get_keyboard_event_source());

	_isRunning = true;
	_render = false;
	_gameTime = 0;
	_cycles = 0;
	_render_frames = 0;
	_animation_frame = 0;

	_FPS = 0;
	_CPS = 0;
}

AppCtl::~AppCtl() {
	for (auto it = _sprites.begin(); it != _sprites.end(); ++it) {
		al_destroy_bitmap(*it);
	}

	if (_font)
		al_destroy_font(_font);

	al_destroy_timer(_timer);
	al_destroy_event_queue(_eventQueue);
	al_destroy_display(_display);
}

void AppCtl::render() {

}

void AppCtl::update() {

}

void AppCtl::controlLoop() {
	al_start_timer(_timer);
	_gameTime = al_current_time();
	auto current_frame = _sprites.begin();
	current_frame++;

	double animation_speed = 100;
	TaskTimer frame_t(ANIMATION_TICKS);
	TaskTimer render_t(CLOCK_SPEED / 60);
	Sprite grass(0, "res/grass.png");

	while (_isRunning) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(_eventQueue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				_isRunning = false;
				break;
			case ALLEGRO_KEY_LEFT:
				break;
			case ALLEGRO_KEY_RIGHT:
				break;
			case ALLEGRO_KEY_UP:
				animation_speed += 10;
				frame_t.adjust(ANIMATION_TICKS / animation_speed * 100);
				break;
			case ALLEGRO_KEY_DOWN:
				if (animation_speed > 10) {
					animation_speed -= 10;
					frame_t.adjust(ANIMATION_TICKS / animation_speed * 100);
				}
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			_cycles++;

			if (al_current_time() - _gameTime >= 1) {
				_gameTime = al_current_time();
				_CPS = _cycles;
				_FPS = _render_frames;
				_cycles = _render_frames = 0;
			}

			if (render_t.check()) {
				_render_frames++;
				_render = true;
				render_t.relaunch();
			}

			if (frame_t.check()) {
				_animation_frame++;
				if (_animation_frame > 20) _animation_frame = 0;

				current_frame++;
				if (current_frame == _sprites.end()) {
					current_frame = _sprites.begin();
					current_frame++;
				}
				frame_t.relaunch();
			}
		}

		//==============================================
		//RENDER
		//==============================================
		if (_render && al_is_event_queue_empty(_eventQueue)) {
			_render = false;


			int xOffset = 368;
			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 20; j++) {
					grass.draw(xOffset + XtoISO(i * 32, j * 32), YtoISO(i * 32, j * 32));
				}
			}
			al_draw_bitmap(*current_frame, 0, 0, 0);

			string timeSTR("Game time: " + to_string(static_cast<long long>(_gameTime)));
			string cpsSTR("Cycles per second: " + to_string(static_cast<long long>(_CPS)));
			string fpsSTR("Frames per second: " + to_string(static_cast<long long>(_FPS)));
			string spdSTR("Animation speed: " + to_string(static_cast<long long>(animation_speed)) + "%");
			string frameSTR("Animation frame #" + to_string(static_cast<long long>(_animation_frame)));
			al_draw_text(_font, al_map_rgb(255, 255, 255), 5, 5, 0, timeSTR.c_str());
			al_draw_text(_font, al_map_rgb(255, 255, 255), 5, 30, 0, cpsSTR.c_str());
			al_draw_text(_font, al_map_rgb(255, 255, 255), 5, 55, 0, fpsSTR.c_str());
			al_draw_text(_font, al_map_rgb(255, 255, 255), 5, 80, 0, spdSTR.c_str());
			al_draw_text(_font, al_map_rgb(255, 255, 255), 5, 105, 0, frameSTR.c_str());

			//FLIP BUFFERS========================
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}
}


int main(int argc, char **argv) {

	if (!al_init()) {
		cout << "failed to initialize allegro!" << endl;
		return -1;
	}

	if (!al_install_keyboard()) {
		cout << "failed to initialize keyboard!" << endl;
		return -1;
	}

	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	AppCtl app;
	app.controlLoop();

	return 0;
}