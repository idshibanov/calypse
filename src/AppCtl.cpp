#include <vector>
#include <string>

#include "AppCtl.h"
#include "Timer.h"

AppCtl::AppCtl() {
	_map = make_shared<LocalMap>();
	_pFinder = make_shared<AStarSearch>(_map);
	_map->generate(_pFinder);

	_stats = make_shared<AppStats>();
	_camera = make_shared<Camera>(TD_MAP_COLS*TD_TILESIZE_X, TD_MAP_ROWS*TD_TILESIZE_Y);
	_mouse = make_shared<Mouse>();
	_screen = new ScreenCtl(_map, _camera, _mouse, _stats);

	_eventQueue = al_create_event_queue();
	_timer = al_create_timer(1.0 / CLOCK_SPEED);
	al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));
	al_register_event_source(_eventQueue, al_get_keyboard_event_source());
	al_register_event_source(_eventQueue, al_get_mouse_event_source());

	_isRunning = true;
	_render = false;
	_gameTime = 0;
	_cycles = 0;
	_render_frames = 0;
	_animation_frame = 0;

	_stats->_FPS = 0;
	_stats->_CPS = 0;
	_keys = new bool[7];
	for (int i = 0; i < 7; i++) _keys[i] = false;
}

AppCtl::~AppCtl() {
	al_destroy_timer(_timer);
	al_destroy_event_queue(_eventQueue);
	delete _screen;
	delete _keys;
}

void AppCtl::render() {

}

void AppCtl::update() {
	unsigned dist = 2;
	if (_keys[SHIFT]) dist *= 10;

	if (_keys[LEFT]) {
		_camera->move(1, dist);
	} else if (_keys[RIGHT]) {
		_camera->move(2, dist);
	}
	if (_keys[UP]) {
		_camera->move(3, dist);
	} else if (_keys[DOWN]) {
		_camera->move(4, dist);
	}

	if (_camera->isUpdated()) {
		_screen->redraw(true);
	}
}

void AppCtl::controlLoop() {
	al_start_timer(_timer);
	_stats->_gameTime = al_current_time();

	double animation_speed = 100;
	TaskTimer render_t(CLOCK_SPEED / 60);

	while (_isRunning) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(_eventQueue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				_isRunning = false;
				break;
			case ALLEGRO_KEY_LEFT:
				_keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				_keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				_keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				_keys[DOWN] = true;
				break;
			}
		} else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_LEFT:
				_keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				_keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_UP:
				_keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				_keys[DOWN] = false;
				break;
			}
		} else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
			if (_mouse->set(ev.mouse.x, ev.mouse.y)) {
				_screen->redraw();
			}

			if (ev.mouse.dz < 0) {
				_screen->zoomOut();
			} else if (ev.mouse.dz > 0) {
				_screen->zoomIn();
			}
		} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			int absX = ev.mouse.x;
			int absY = ev.mouse.y;
			_mouse->set(absX, absY, ev.mouse.button, true);
			int tileX = _screen->convertScreenX(absX, absY);
			int tileY = _screen->convertScreenY(absX, absY);
			cout << tileX << "," << tileY << endl;
			auto actor = _map->getActor();
			if (tileX > 0 && tileY > 0) {
				actor->setXPos(tileX);
				actor->setYPos(tileY);
			}
		} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			_mouse->setPressed(false);
		} else if (ev.type == ALLEGRO_EVENT_TIMER) {
			_cycles++;

			if (al_current_time() - _stats->_gameTime >= 1) {
				_stats->_gameTime = al_current_time();
				_stats->_CPS = _cycles;
				_stats->_FPS = _render_frames;
				_cycles = _render_frames = 0;
			}

			if (render_t.check()) {
				_render = true;
				update();
				render_t.relaunch();
			}

			_screen->updateTimers();
		}

		if (_render && al_is_event_queue_empty(_eventQueue)) {
			_render_frames++;
			_render = false;

			if (_screen->draw()) {
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
			}
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

	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	AppCtl app;
	app.controlLoop();

	return 0;
}