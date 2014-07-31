#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "gamectl.h"
#include "critter.h"
#include "pathfinder.h"

GameCtl::GameCtl(){
    _map = make_shared<GameMap>();
    _pFinder = make_shared<AStarSearch>(_map);
    _map->generate(_pFinder);

    _cam = new TCamera(TD_MAP_COLS*TD_TILESIZE_X, TD_MAP_ROWS*TD_TILESIZE_Y);
    _cursor = new TCursor(0,0);
    _mouse = new TMouse();
    _screen = new ScreenCtl(_map, _cam, _cursor);

	_eventQueue = al_create_event_queue();
	_timer = al_create_timer(1.0 / 60);

    _isRunning = false;
	_render = false;
	_gameTime = 0;
	_frames = 0;
	_gameFPS = 0;
    _actionID = -1;

    _keys = new bool[7];
    for(int i = 0; i < 7; i++) _keys[i] = false;
}

GameCtl::~GameCtl(){
    delete _screen;
    delete _cam;
    delete _cursor;
    delete _mouse;
    delete [] _keys;
	al_destroy_timer(_timer);
	al_destroy_event_queue(_eventQueue);
}

void GameCtl::gameRender(){
    _screen->draw(*_mouse, _gameFPS);
}

void GameCtl::gameInit(){

}

void GameCtl::gameUpdate(){
    _map->update();
    _screen->update();

    unsigned dist = 4;
    if(_keys[SHIFT]) dist *= 10;

    if(_keys[LEFT]){
        _cam->move(1, dist);
    }else if(_keys[RIGHT]){
        _cam->move(2, dist);
    }
    if(_keys[UP]){
        _cam->move(3, dist);
    } else if(_keys[DOWN]) {
        _cam->move(4, dist);
    }

    if(_actionID != -1){
        cout << "Button action: " << _actionID << endl;

        int tileType = 0;
        switch(_actionID){
        case 1:
            tileType = rand() % 160;
            break;
        case 2:
            tileType = 8 + (rand() % 6);
            break;
        }

        _map->setTile(_map->getColMax() * 10 + 10, tileType);
        _actionID = -1;
    }
}

void GameCtl::gameRun(){
    std::vector<AStarNode> path = _pFinder->searchPath(151, 460);
    for (auto it = path.begin(); it != path.end(); ++it) {
        std::cout << (*it) << endl;
    }
    std::cout << endl;
    path = _pFinder->searchPath(452, 4);
    for (auto it = path.begin(); it != path.end(); ++it) {
        std::cout << (*it) << endl;
    }

    al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));
	al_register_event_source(_eventQueue, al_get_keyboard_event_source());
    al_register_event_source(_eventQueue, al_get_mouse_event_source());

    al_start_timer(_timer);
	_gameTime = al_current_time();
    gameInit();
    _isRunning = true;
    while(_isRunning) {
        ALLEGRO_EVENT ev;
		al_wait_for_event(_eventQueue, &ev);

		// INPUT
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch(ev.keyboard.keycode)
			{
            case ALLEGRO_KEY_ESCAPE:
                _isRunning = false;
                break;
            case ALLEGRO_KEY_LEFT:
                if(_cursor->getXPos() > 0)
                    _cursor->decreaseX();
                break;
            case ALLEGRO_KEY_RIGHT:
                if(_cursor->getXPos() < TD_MAP_COLS)
                    _cursor->increaseX();
                break;
            case ALLEGRO_KEY_UP:
                if(_cursor->getYPos() > 0)
                    _cursor->decreaseY();
                break;
            case ALLEGRO_KEY_DOWN:
                if(_cursor->getYPos() < TD_MAP_ROWS)
                    _cursor->increaseY();
                break;
            case ALLEGRO_KEY_A:
			case ALLEGRO_KEY_PAD_4:
				_keys[LEFT] = true;
				break;
            case ALLEGRO_KEY_D:
			case ALLEGRO_KEY_PAD_6:
				_keys[RIGHT] = true;
				break;
            case ALLEGRO_KEY_W:
			case ALLEGRO_KEY_PAD_8:
				_keys[UP] = true;
				break;
            case ALLEGRO_KEY_S:
			case ALLEGRO_KEY_PAD_2:
				_keys[DOWN] = true;
				break;
            case ALLEGRO_KEY_LSHIFT:
            case ALLEGRO_KEY_RSHIFT:
				_keys[SHIFT] = true;
                break;
            case ALLEGRO_KEY_LCTRL:
            case ALLEGRO_KEY_RCTRL:
				_keys[CTRL] = true;
                break;
            case ALLEGRO_KEY_ALT:
            case ALLEGRO_KEY_ALTGR:
				_keys[ALT] = true;
                break;
            case ALLEGRO_KEY_SPACE:
                break;
            }
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				_isRunning = false;
				break;
			case ALLEGRO_KEY_LEFT:
				break;
			case ALLEGRO_KEY_RIGHT:
				break;
			case ALLEGRO_KEY_UP:
				break;
			case ALLEGRO_KEY_DOWN:
				break;
            case ALLEGRO_KEY_A:
			case ALLEGRO_KEY_PAD_4:
				_keys[LEFT] = false;
				break;
            case ALLEGRO_KEY_D:
			case ALLEGRO_KEY_PAD_6:
				_keys[RIGHT] = false;
				break;
            case ALLEGRO_KEY_W:
			case ALLEGRO_KEY_PAD_8:
				_keys[UP] = false;
				break;
            case ALLEGRO_KEY_S:
			case ALLEGRO_KEY_PAD_2:
				_keys[DOWN] = false;
				break;
            case ALLEGRO_KEY_LSHIFT:
            case ALLEGRO_KEY_RSHIFT:
				_keys[SHIFT] = false;
                break;
            case ALLEGRO_KEY_LCTRL:
            case ALLEGRO_KEY_RCTRL:
				_keys[CTRL] = false;
                break;
            case ALLEGRO_KEY_ALT:
            case ALLEGRO_KEY_ALTGR:
				_keys[ALT] = false;
                break;
			case ALLEGRO_KEY_SPACE:
				break;
			}
		} else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            _mouse->set(ev.mouse.x, ev.mouse.y);
            if(ev.mouse.dz < 0) {
                _screen->zoomOut();
            } else if (ev.mouse.dz > 0) {
                _screen->zoomIn();
            }
		} else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            int absX = ev.mouse.x;
            int absY = ev.mouse.y;
            _mouse->set(absX, absY, ev.mouse.button, true);

            if(_screen->isGameFrame(absX, absY)){
                processScreenAction(*_mouse);
            } else {
                processUIAction(absX, absY, _mouse->getButton());
            }

        } else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            _mouse->setPressed(false);
		//==============================================
		//GAME UPDATE
		//==============================================
        } else if(ev.type == ALLEGRO_EVENT_TIMER) {
			_render = true;
			_frames++;
			if(al_current_time() - _gameTime >= 1)
			{
				_gameTime = al_current_time();
				_gameFPS = _frames;
				_frames = 0;
			}
            gameUpdate();
		}

		//==============================================
		//RENDER
		//==============================================
		if(_render && al_is_event_queue_empty(_eventQueue))
		{
			_render = false;
            // display FPS on screen
			// al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", gameFPS);	

			//BEGIN PROJECT RENDER================
            gameRender();

			//FLIP BUFFERS========================
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}
}

void GameCtl::processScreenAction(TMouse& mouse) {
    int absX = mouse.getXPos();
    int absY = mouse.getYPos();
    int button = mouse.getButton();
    float tileX = _screen->convertScreenX(absX);
    float tileY = _screen->convertScreenY(absY);

    if (tileX > 0 && tileY > 0) {
        if (button == TMOUSE_BUTTON_LEFT) {
            if (mouse.getSpecialAction() == TMOUSE_SPECIAL_ACTION_NONE) {
                weak_ptr<GameObject> obj = _map->getObject(tileX, tileY);
                if(obj.expired()){
                    // move character
                    _map->getCritter().lock()->setDest(tileX, tileY);
                } else {
                
                }
            } else {
                // switch & do certain action
            }
        } else if (button == TMOUSE_BUTTON_RIGHT) {
            _screen->processAction(absX, absY, button);
        }
    }
}

void GameCtl::processUIAction(int absX, int absY, int button) {
    _actionID = _screen->processUI(absX, absY);

}



int main() {
    if(!al_init()) {
        return -1;
    }
    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    GameCtl ctl;
    ctl.gameRun();

    return 0;
}
