#include <vector>
#include <string>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#include "AppCtl.h"
#include "Timer.h"
#include "Object.h"

AppCtl::AppCtl() {
	_state = make_shared<AppState>();
	_config = make_shared<ConfigCtl>();
	_res = make_shared<ResourceCtl>(_config);
	_map = make_shared<LocalMap>(_res);

	auto set = _config->getSetting(C_CONFIG_APP, "resolution");
	_state->_resolution = extractWithDefault<Point>(set, Point(TD_DISPLAY_WIDTH, TD_DISPLAY_HEIGHT));

	//al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	//al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_REQUIRE);
	//al_set_new_display_flags(ALLEGRO_NOFRAME);

	_display = al_create_display(_state->_resolution._x, _state->_resolution._y);
	al_hide_mouse_cursor(_display);

	// load sprites only after setting up display
	_res->loadResources();
	//_res->loadSprites();

	_camera = make_shared<Camera>(TD_MAP_COLS*TD_TILESIZE_X, TD_MAP_ROWS*TD_TILESIZE_Y);
	_mouse = make_shared<Mouse>();

	_state->_appSpeed = 100;
	_state->_FPS = 0;
	_state->_CPS = 0;
	_state->_drawStats = false;
	_state->_drawGrid = false;
	_state->_drawCoords = false;
	_state->_drawMasks = false;
	_state->_drawUIAreas = false;

	_state->_curScreen = nullptr;
	_state->_selectedAction = ACTION_NONE;
	_state->_selectedObject = -1;
	_state->_selectedFrame = nullptr;
	_keys = new bool[CALYPSE_KEYS_LAST];
	for (int i = 0; i < CALYPSE_KEYS_LAST; i++) _keys[i] = false;


	_screen = new ScreenCtl(_res, _map, _camera, _mouse, _state);

	_pFinder = std::make_shared<AStarSearch>(_map);
	_events = make_shared<EventService>(_res, _map, _pFinder, _state);
	_map->generate(_pFinder, _events);

	int sID = 0;
	auto vec = _config->getCollection(C_CONFIG_UI, "screens");
	for (auto sInfo : vec) {
		auto sObj = std::dynamic_pointer_cast<JsonObject>(sInfo.second);
		if (sObj) {
			_screenMap.emplace(sID, make_shared<Screen>(_res, *sObj));
			_screenLookup.emplace(sInfo.first, sID);
			sID++;
		}
	}
	switchScreen("main");
	_screen->reloadScreen();

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
}

AppCtl::~AppCtl() {
	al_destroy_timer(_timer);
	al_destroy_event_queue(_eventQueue);
	al_destroy_display(_display);
	delete _screen;
	delete _keys;
}

void AppCtl::render() {

}

void AppCtl::update() {
	unsigned dist = 2;
	if (_keys[SHIFT]) dist *= 3;

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
	_map->update();
}

void AppCtl::controlLoop() {
	al_start_timer(_timer);
	_state->_appTime = al_current_time();

	double animation_speed = 100;
	TaskTimer render_t(CLOCK_SPEED / 60);

	while (_isRunning) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(_eventQueue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
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
			case ALLEGRO_KEY_C:
				_screen->toggleInfoScreen();
				break;
			case ALLEGRO_KEY_I:
				_screen->toggleInventory();
				break;
			case ALLEGRO_KEY_0:
				selectAction(ACTION_NONE);
				break;
			case ALLEGRO_KEY_1:
				selectAction(ACTION_DRAG);
				break;
			case ALLEGRO_KEY_2:
				selectAction(ACTION_CRAFT_CAMPFIRE);
				break;
			case ALLEGRO_KEY_3:
				selectAction(ACTION_CRAFT_TREE);
				break;
			case ALLEGRO_KEY_F8:
				_state->_drawMasks = !_state->_drawMasks;
				break;
			case ALLEGRO_KEY_F9:
				_state->_drawGrid = !_state->_drawGrid;
				break;
			case ALLEGRO_KEY_F10:
				_state->_drawCoords = !_state->_drawCoords;
				break;
			case ALLEGRO_KEY_F11:
				_state->_drawUIAreas = !_state->_drawUIAreas;
				break;
			case ALLEGRO_KEY_F12:
				_state->_drawStats = !_state->_drawStats;
				break;
			case ALLEGRO_KEY_LSHIFT:
			case ALLEGRO_KEY_RSHIFT:
				_keys[SHIFT] = true;
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
			case ALLEGRO_KEY_LSHIFT:
			case ALLEGRO_KEY_RSHIFT:
				_keys[SHIFT] = false;
				break;
			}

		} else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
			if (_mouse->set(Point(ev.mouse.x, ev.mouse.y))) {
				if (_state->_selectedFrame) {
					_state->_selectedFrame->move(_mouse->getDelta());
				}
				_screen->redraw();
			}

			if (ev.mouse.dz < 0) {
				_screen->zoomOut();
			} else if (ev.mouse.dz > 0) {
				_screen->zoomIn();
			}

		} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			_mouse->set(Point(ev.mouse.x, ev.mouse.y), ev.mouse.button, true);
			processMouseAction();

		} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			_mouse->setPressed(false);
			_state->_selectedFrame = nullptr;

		} else if (ev.type == ALLEGRO_EVENT_TIMER) {
			_cycles++;

			if (al_current_time() - _state->_appTime >= 1) {
				_state->_appTime = al_current_time();
				_state->_CPS = _cycles;
				_state->_FPS = _render_frames;
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


bool AppCtl::switchScreen(const std::string& name) {
	auto it = _screenLookup.find(name);
	if (it != _screenLookup.end()) {
		auto scr = _screenMap.find(it->second);
		if (scr != _screenMap.end()) {
			_state->_curScreen = scr->second;
			return true;
		}
	}
	return false;
}


void AppCtl::processMouseAction() {
	Point absPos = _mouse->getPos();
	Point clickPos = _screen->convertCoords(absPos);
	auto actor = _map->getPrimaryActor();

	auto elem = _screen->processAction(absPos);
	cout << endl << "Click on: " << absPos._x << "," << absPos._y << endl;
	if (_mouse->getButton() == MOUSE_BUTTON_LEFT) {
		if (_state->_selectedAction > ACTION_NONE) {
			Point mapClick = clickPos.div(SUBTILE_MASK).mul(SUBTILE_MASK);
			if (mapClick > 0) {
				Point mvTarget = _pFinder->findAdjacent(actor->getPos(), Rect(mapClick, Point(10, 10)));
				auto act1 = make_shared<MoveAction>(ACTION_MOVE, _res, actor, 8, 8, mvTarget, _pFinder);

				auto act2 = make_shared<PointAction>(_state->_selectedAction, _res, actor, 15, 8, clickPos, _map);
				act1->chainAction(act2);

				actor->setAction(act1);
			}
		} else if (elem) {
			if (elem->getType() == AREA_TYPE_FRAME) {
				auto selFrame = std::dynamic_pointer_cast<UIFrame>(elem);
				_state->_selectedFrame = selFrame.get();
			} else if (elem->getType() == AREA_TYPE_ELEMENT) {
				auto uiEl = std::dynamic_pointer_cast<UIElement>(elem);
				processUIElement(uiEl);
			} else if (elem->getType() == AREA_TYPE_ITEM) {
				/*
				auto gItemArea = std::dynamic_pointer_cast<ItemArea>(elem);

				auto gItem = _map->getItem(gItemArea->_item->getID());
				if (gItem) {
				if (actor->getState()->getInventory()->addItem(gItem)) {
				_map->removeItem(gItem->getID());
				}
				}
				*/
				auto gItemArea = std::dynamic_pointer_cast<ItemArea>(elem);
				Point mapClick = clickPos.div(SUBTILE_MASK).mul(SUBTILE_MASK);
				if (mapClick > 0) {
					Point mvTarget = _pFinder->findAdjacent(actor->getPos(), Rect(mapClick, Point(10, 10)));
					auto act1 = make_shared<MoveAction>(ACTION_MOVE, _res, actor, 8, 8, mvTarget, _pFinder);

					auto act2 = make_shared<ItemAction>(ACTION_PICK_ITEM, _res, actor, 10, 1, gItemArea->_item, _map);
					act1->chainAction(act2);

					actor->setAction(act1);
				}
			} else if (elem->getType() == AREA_TYPE_OBJECT) {
				auto obj_ptr = std::dynamic_pointer_cast<ObjectArea>(elem);
				auto obj = obj_ptr->_obj;
				auto subAreas = obj_ptr->getSubArea(absPos);

				if (subAreas) {
					_state->_selectedObject = obj->getID();
					_screen->displayOptions(obj->getPos(), subAreas);
				} else {
					//_events->process(obj, ACTION_DRAG);
				}
			}
		} else if (_state->_selectedItem) {
			_map->putItem(actor->getPos(), _state->_selectedItem);
			selectItem(nullptr);
		} else {
			cout << clickPos._x << "," << clickPos._y << endl;
			if (clickPos > 0) {
				auto act = make_shared<MoveAction>(ACTION_MOVE, _res, actor, 8, 8, clickPos, _pFinder);
				actor->setAction(act);
			}
		}
	} else if (_mouse->getButton() == MOUSE_BUTTON_RIGHT) {
		if (_state->_selectedAction > ACTION_NONE) {
			_state->_selectedAction = ACTION_NONE;
		} else if (clickPos._x > 0 && clickPos._y > 0 && actor->isHolding()) {
			Rect clickArea(clickPos, Point(2, 2));
			Point target = _pFinder->findAdjacent(actor->getPos(), clickArea);

			if (target._x >= 0 && target._y >= 0) {
				auto act1 = make_shared<MoveAction>(ACTION_MOVE, _res, actor, 8, 8, target, _pFinder);
				auto act2 = make_shared<PointAction>(ACTION_DROP, _res, actor, 1, 1, clickPos, _map);
				act1->chainAction(act2);
				actor->setAction(act1);
			}
		}
	}
}

void AppCtl::processUIElement(shared_ptr<UIElement> elem) {
	if (elem->getElementType() == UIELEMENT_TYPE_BUTTON) {
		auto button = std::dynamic_pointer_cast<UIButton>(elem);
		if (button) {
			int actionID = button->getActionID();
			if (actionID > ACTION_NONE && actionID < ACTION_LAST) {
				auto parentFrame = button->getParent();
				if (parentFrame) {
					if (actionID == ACTION_CLOSE_PARENT) {
						parentFrame->setVisible(false);
					}
				} else {
					_screen->hideOptions();
					_events->process((ActionType)actionID);
				}
			}
			button->launchTimer();
		}
	} else if (elem->getElementType() == UIELEMENT_TYPE_CONTAINER) {
		auto container = std::dynamic_pointer_cast<ContainerArea>(elem);

		Point cell = container->getCell(_mouse->getPos());
		cout << "Cell addr: " << cell._x << "," << cell._y << endl;
		auto inv = _map->getPrimaryActor()->getState()->getInventory();

		auto itemID = inv->checkCell(cell);

		if (_state->_selectedItem) {
			selectItem(inv->forceItem(_state->_selectedItem, cell));
		} else if (itemID != -1) {
			selectItem(inv->takeItem(itemID));
		} else {
			// TODO: remove later on
			// generate stuff for debugging purposes
			int randomID = rand() % _res->getItemCount();
			inv->putItem(make_shared<Item>(randomID), cell);
		}
	}
}


void AppCtl::selectAction(ActionType t) {
	if (!_state->_selectedItem) {
		if (t == ACTION_DRAG) {
			_mouse->setSprite(1);
		} else {
			_mouse->setSprite(0);
		}
		_state->_selectedAction = t;
	}
}

void AppCtl::selectItem(shared_ptr<Item> it) {
	if (_state->_selectedAction == ACTION_NONE) {
		_state->_selectedItem = it;
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
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	try {
		AppCtl app;
		app.controlLoop();
	} catch (std::string& e) {
		std::cerr << e << endl;
		al_show_native_message_box(
			NULL,
			"Warning",
			"Are you sure?",
			e.c_str(),
			NULL,
			ALLEGRO_MESSAGEBOX_ERROR
			);
	}

	return 0;
}