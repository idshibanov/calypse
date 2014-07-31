#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <cstdlib>
#include <iostream>
#include "sprite.h"
#include "map.h"
#include "camera.h"
#include "frame.h"
#include "main.h"

int curx, cury;
int renderX, renderY;
int tileCol, tileRow;

bool keys[] = {false, false, false, false};
enum KEYS{
    UP, 
    DOWN, 
    LEFT,
    RIGHT
};

void init_game(GameMap* map){
    curx = 0;
    cury = 0;
    renderX = 0;
    renderY = 0;
    tileCol = 0;
    tileRow = 0;
}

void update_game(TDCamera* cam){
    if(keys[LEFT]){
        cam->move(1);
    }else if(keys[RIGHT]){
        cam->move(2);
    }
    if(keys[UP]){
        cam->move(3);
    } else if(keys[DOWN]) {
        cam->move(4);
    }
}

void display_game(SpriteSheet* spr, GameMap* map, TDCamera* cam){
    // map size
    unsigned rowmax = map->getRowMax();
    unsigned colmax = map->getColMax();
    // camera position
    unsigned camX = cam->getXPos();
    unsigned camY = cam->getYPos();
    // first tile to render - coords and array id
    unsigned firstTileX = 0, firstTileY = 0;
    unsigned maxTileCol = 0, maxTileRow = 0;
    tileCol = 0;
    tileRow = 0;
    // display coords to start rendering
    renderX = 0;
    renderY = 0;
    

    if(camX < TD_DISPLAY_WIDTH / 2){
        // adjust render X position (left border)
        renderX = TD_DISPLAY_WIDTH / 2 - camX;
    } else {
        // find out first tile to render
        firstTileX = camX - TD_DISPLAY_WIDTH / 2;
        tileCol = firstTileX / TD_TILESIZE_X;

        // adjust render X position if cam X coord is uneven
        if(firstTileX % TD_TILESIZE_X){
            renderX = -(firstTileX % TD_TILESIZE_X);
        }
    }

    if(camY < TD_DISPLAY_HEIGHT / 2){
        // adjust render Y position (top border)
        renderY = TD_DISPLAY_HEIGHT / 2 - camY;
    } else {
        // find out first tile to render
        firstTileY = camY - TD_DISPLAY_HEIGHT / 2;
        tileRow = firstTileY / TD_TILESIZE_Y;

        // adjust render Y position if cam Y coord is uneven
        if(firstTileY % TD_TILESIZE_Y){
            renderY = -(firstTileY % TD_TILESIZE_Y);
        }
    }

    maxTileCol = tileCol + ((renderX + TD_DISPLAY_WIDTH) / TD_TILESIZE_X) + 2;
    if (colmax < maxTileCol) maxTileCol = colmax;
    maxTileRow = tileRow + ((renderY + TD_DISPLAY_HEIGHT) / TD_TILESIZE_Y) + 2;
    if (rowmax < maxTileRow) maxTileRow = rowmax;
    
    for(int row = tileRow; row < maxTileRow; row++){
        for(int col = tileCol; col < maxTileCol; col++){
            spr->draw(renderX + ((col - tileCol) * TD_TILESIZE_X), 
                      renderY + ((row - tileRow) * TD_TILESIZE_Y), 
                      map->getTile(row*colmax+col) );
        }
    }
    spr->draw(TD_DISPLAY_WIDTH / 2, TD_DISPLAY_HEIGHT / 2, 51);

    if(curx >= tileCol && cury >= tileRow && curx <= maxTileCol && cury <= maxTileRow){
        //spr->draw(renderX + (curx * TD_TILESIZE_X), renderY + (cury * TD_TILESIZE_Y), 52);
        spr->draw(renderX + ((curx - tileCol) * TD_TILESIZE_X), 
                  renderY + ((cury - tileRow) * TD_TILESIZE_Y), 52 );
    }
}

int main() {
    // gameloop variables
    bool done = false;
	bool render = false;
	float gameTime = 0;
	int frames = 0;
	int gameFPS = 0;

    // allegro variables
    ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;

    // allegro init
    if(!al_init()) {
        return -1;
    }
    display = al_create_display(TD_DISPLAY_WIDTH, TD_DISPLAY_HEIGHT);
    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();

    // variables init and startup
    SpriteSheet spr(0, "dustvg.png", 207, 16);
    GameMap* _map = new GameMap();
    TDCamera* cam = new TDCamera(TD_MAP_COLS*32, TD_MAP_ROWS*32);

    event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);
	gameTime = al_current_time();

    // GAME LOOP
    init_game(_map);
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		// INPUT
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_LEFT:
                if(curx > 0)
                    curx--;
                break;
            case ALLEGRO_KEY_RIGHT:
                if(curx < TD_MAP_COLS)
                    curx++;
                break;
            case ALLEGRO_KEY_UP:
                if(cury > 0)
                    cury--;
                break;
            case ALLEGRO_KEY_DOWN:
                if(cury < TD_MAP_ROWS)
                    cury++;
                break;
            case ALLEGRO_KEY_A:
			case ALLEGRO_KEY_PAD_4:
				keys[LEFT] = true;
				break;
            case ALLEGRO_KEY_D:
			case ALLEGRO_KEY_PAD_6:
				keys[RIGHT] = true;
				break;
            case ALLEGRO_KEY_W:
			case ALLEGRO_KEY_PAD_8:
				keys[UP] = true;
				break;
            case ALLEGRO_KEY_S:
			case ALLEGRO_KEY_PAD_2:
				keys[DOWN] = true;
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
				done = true;
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
				keys[LEFT] = false;
				break;
            case ALLEGRO_KEY_D:
			case ALLEGRO_KEY_PAD_6:
				keys[RIGHT] = false;
				break;
            case ALLEGRO_KEY_W:
			case ALLEGRO_KEY_PAD_8:
				keys[UP] = false;
				break;
            case ALLEGRO_KEY_S:
			case ALLEGRO_KEY_PAD_2:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				break;
			}
		}
        else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            curx = (ev.mouse.x - renderX) / 32 + tileCol;
            cury = (ev.mouse.y - renderY) / 32 + tileRow;
        }
		//==============================================
		//GAME UPDATE
		//==============================================
		else if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			render = true;
			frames++;
			if(al_current_time() - gameTime >= 1)
			{
				gameTime = al_current_time();
				gameFPS = frames;
				frames = 0;
			}
            update_game(cam);
		}

		//==============================================
		//RENDER
		//==============================================
		if(render && al_is_event_queue_empty(event_queue))
		{
			render = false;
            // display FPS on screen
			// al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", gameFPS);	

			//BEGIN PROJECT RENDER================
            display_game(&spr, _map, cam);

			//FLIP BUFFERS========================
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
        //al_rest(1.0);
	}

    // variables cleanup
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    delete _map;
    return 0;
}