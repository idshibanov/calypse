#include <vector>
#include <string>

#include "AppCtl.h"
#include "Timer.h"

AppCtl::AppCtl() {
	_map = make_shared<LocalMap>();
	_pFinder = make_shared<AStarSearch>(_map);
	_map->generate(_pFinder);

	_camera = make_shared<Camera>(TD_MAP_COLS*TD_TILESIZE_X, TD_MAP_ROWS*TD_TILESIZE_Y);
	_screen = new ScreenCtl(_map, _camera);

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
	al_destroy_timer(_timer);
	al_destroy_event_queue(_eventQueue);
	delete _screen;
}

void AppCtl::render() {

}

void AppCtl::update() {

}

void AppCtl::controlLoop() {
	al_start_timer(_timer);
	_gameTime = al_current_time();

	double animation_speed = 100;
	TaskTimer render_t(CLOCK_SPEED / 60);
	AppStats stats;

	while (_isRunning) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(_eventQueue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				_isRunning = false;
				break;
			case ALLEGRO_KEY_LEFT:
				_camera->move(1, 10);
				break;
			case ALLEGRO_KEY_RIGHT:
				_camera->move(2, 10);
				break;
			case ALLEGRO_KEY_UP:
				_camera->move(3, 10);
				//_screen->increaseSpeed();
				break;
			case ALLEGRO_KEY_DOWN:
				_camera->move(4, 10);
				//_screen->decreaseSpeed();
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
				_render = true;
				render_t.relaunch();
			}

			_screen->updateTimers();
		}

		//==============================================
		//RENDER
		//==============================================
		if (_render && al_is_event_queue_empty(_eventQueue)) {
			_render_frames++;
			_render = false;

			stats._CPS = _CPS;
			stats._FPS = _FPS;
			stats._gameTime = _gameTime;
			_screen->draw(stats);

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