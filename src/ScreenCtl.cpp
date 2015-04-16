#include <iomanip>
#include <sstream>
#include <allegro5/allegro_primitives.h>

#include "ScreenCtl.h"
#include "Object.h"

ScreenCtl::ScreenCtl (shared_ptr<ResourceCtl> res, shared_ptr<LocalMap> map, shared_ptr<Camera> cam, 
	                  shared_ptr<Mouse> mouse, shared_ptr<AppState> stats)
					  : frame_t(ANIMATION_TICKS) {
	//al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	//al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_REQUIRE);

	_display = al_create_display(TD_DISPLAY_WIDTH, TD_DISPLAY_HEIGHT);
	al_hide_mouse_cursor(_display);

	// load sprites only after setting up display
	res->loadSprites();

	_screenWidth = TD_DISPLAY_WIDTH;
	_screenHeight = TD_DISPLAY_HEIGHT;
	_tileWidth = TD_TILESIZE_X;
	_tileHeight = TD_TILESIZE_Y;
	_tileSize.set(_tileWidth, _tileHeight);

	_res = res;
	_map = map;
	_cam = cam;
	_mouse = mouse;
	_state = stats;

	_render = true;
	_lastTimestamp = _state->_appTime;

	_zoom = 1.0;

	_menu = make_shared<CarouselMenu>(Point(300, 200), Point(100, 100), nullptr);

	_font = make_shared<SpriteText>("res/arialbd.ttf", 12);

	_button = make_shared<UIButton>(Point(700, 0), nullptr, Point(100, 50), -1, _res->getSprite(6),
		_res->getSprite(7), _font, std::string("Label"));

	_animation_speed = 100;
	_animation_frame = 0;
}

ScreenCtl::~ScreenCtl() {
	al_destroy_display(_display);
}

bool ScreenCtl::draw() {
	bool retval = false;
	if (_render || _lastTimestamp < _state->_appTime) {
		_buffer.reset();
		ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);
		ALLEGRO_COLOR obj_color = al_map_rgb(155, 155, 255);
		// map size
		unsigned rowmax = _map->getRowMax();
		unsigned colmax = _map->getColMax();
		Point isoTileSize(_tileSize._x * 2, _tileSize._y);

		// Map tiles
		auto grass = _res->getSprite(1).get();
		for (int row = _firstTile._y; row < _lastTile._y; row++) {
			for (int col = _firstTile._x; col < _lastTile._x; col++) {
				Point coord(col, row);
				coord = (coord - _firstTile) * _tileSize + _offset;
				coord = coord.toIso() + _screenOffset;

				// loop drawing sub bitmaps must be the same parent
				//al_hold_bitmap_drawing(true);
				grass->drawScaled(coord, isoTileSize);
				//al_hold_bitmap_drawing(false);

				if (_state->_drawCoords) {
					std::string tileCoords(std::to_string(col) + ", " + std::to_string(row));
					_font->draw(tileCoords, coord.add(24, 8), color);
				}
			}
		}

		if (_state->_drawGrid) {
			for (int row = _firstTile._y; row < _lastTile._y; row++) {
				Point lineStart(_firstTile._x, row);
				Point lineEnd(_lastTile._x, row);

				lineStart = convertMapCoords(lineStart * TILE_MASK);
				lineEnd = convertMapCoords(lineEnd * TILE_MASK);

				al_draw_line(lineStart._x, lineStart._y, lineEnd._x, lineEnd._y, color, 1);
			}

			for (int col = _firstTile._x; col < _lastTile._x; col++) {
				Point lineStart(col, _firstTile._y);
				Point lineEnd(col, _lastTile._y);

				lineStart = convertMapCoords(lineStart * TILE_MASK);
				lineEnd = convertMapCoords(lineEnd * TILE_MASK);

				al_draw_line(lineStart._x, lineStart._y, lineEnd._x, lineEnd._y, color, 1);
			}
		} else if (_state->_drawMasks) {
			auto allMasks = _map->getObjectMasks();
			int map_size = _map->getColMax() * TILE_MASK;
			auto red_color = al_map_rgb(255, 128, 0);
			for (auto msk : *allMasks) {
				Point top(msk.first % map_size, msk.first / map_size);
				Point bot(top), left(top), right(top);

				convertMapCoords(top);
				convertMapCoords(bot.modAdd(SUBTILE_MASK, SUBTILE_MASK));
				convertMapCoords(left.modAdd(0, SUBTILE_MASK));
				convertMapCoords(right.modAdd(SUBTILE_MASK, 0));

				al_draw_filled_triangle(left._x, left._y, right._x, right._y, top._x, top._y, red_color);
				al_draw_filled_triangle(left._x, left._y, right._x, right._y, bot._x, bot._y, red_color);

				//_font->draw(to_string(msk.second), left.add(5, 0), color);
			}
		}

		shared_ptr<Actor> _actor = _map->getActor();
		auto renderedObjects = _map->getObjects(_firstTile, _lastTile);

		for (auto obj : renderedObjects) {
			Point coord = obj.second->getPos();
			coord = (coord * _tileSize) / TILE_MASK + _offset - (_firstTile * _tileSize);
			coord = coord.toIso() + _screenOffset;

			int type = obj.second->getType();
			auto objInfo = _res->getObjectInfo(type);

			if (objInfo != nullptr) {
				if (objInfo->liftable() == true) {
					if (obj.second->isDragged()) {
						int actYOffset = _res->getObjectInfo(_actor->getType())->offset()._y;
						coord = _actor->getPos();
						coord = (coord * _tileSize) / TILE_MASK + _offset - (_firstTile * _tileSize);
						coord = coord.toIso().modAdd(0, actYOffset - 10) + _screenOffset;
					}
				}
				Point objSize = objInfo->sprSize() * _zoom;
				coord += objInfo->offset() * _zoom;

				if (type == 0) {
					if (_actor->isWorking()) {
						_font->draw(std::to_string(_actor->getProgress()), coord.add(54, 25), color);
					}
				}

				auto objSprite = _res->getSprite(objInfo->spriteID());
				if (objInfo->frames() == 0) {
					objSprite->drawScaled(coord, objSize);

					_buffer.setElement(make_shared<ObjectArea>(coord, objSize, obj.second, objSprite, objInfo));
				} else {
					auto objSpriteSheet = std::dynamic_pointer_cast<SpriteSheet>(objSprite);
					objSpriteSheet->drawScaled(coord, obj.second->getSprite(), _zoom);

					_buffer.setElement(make_shared<ObjectArea>(coord, objSize, obj.second, objSpriteSheet, objInfo));
				}
				//al_draw_rectangle(coord._x, coord._y, coord._x + objSize._x, coord._y + objSize._y,
					//al_map_rgb(255, 100, 100), 1.0);

				//string tileCoords(to_string(obj.second->getXPos()) + ", " + to_string(obj.second->getYPos()));
				//_font->draw(tileCoords, coord.add(12, 30), color);
				//_font->draw(to_string(obj.first), coord.add(12, 30), color);
				_font->draw(std::to_string(obj.second->getID()), coord.add(12, 30), obj_color);
			}
		}
		for (auto it = _options.begin(); it != _options.end(); it++) {
			auto button = *it;
			button->draw();
			_buffer.setElement(button);
		}

		std::string timeSTR("App time: " + std::to_string(_state->_appTime));
		std::string cpsSTR("Cycles per second: " + std::to_string(_state->_CPS));
		std::string fpsSTR("Frames per second: " + std::to_string(_state->_FPS));
		std::string spdSTR("Animation speed: " + std::to_string(static_cast<int>(_animation_speed)) + "%");
		std::string frameSTR("Animation frame #" + std::to_string(_actor->getSprite()));
		_font->draw(timeSTR, Point(5, 5), color);
		_font->draw(cpsSTR, Point(5, 30), color);
		_font->draw(fpsSTR, Point(5, 55), color);
		_font->draw(spdSTR, Point(5, 80), color);
		_font->draw(frameSTR, Point(5, 105), color);

		// Almost last: mouse cursor
		auto cur = std::dynamic_pointer_cast<SpriteSheet>(_res->getSprite(0));
		cur->draw(_mouse->getPos(), _mouse->getSprite());

		_lastTimestamp = _state->_appTime;
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
	Point camPos = _cam->getPos();
	// map rendering offset, based on camera position
	_offset.set(0, 0);

	// offset after which we shift the tiles
	int maxOffset = (_screenWidth / 2 + _screenHeight) / 2;

	_screenOffset.set(_screenWidth / 2 - _tileWidth, _screenHeight / 2);
	_tileSize.set(_tileWidth, _tileHeight);
	_offset -= camPos;

	if (camPos._x > maxOffset + _tileSize._x) {
		_firstTile._x = (camPos._x - maxOffset) / _tileSize._x;
	}
	if (camPos._y > maxOffset + _tileSize._y) {
		_firstTile._y = (camPos._y - maxOffset) / _tileSize._y;
	}
	_offset += (_firstTile * _tileSize);

	unsigned rowmax = _map->getRowMax();
	unsigned colmax = _map->getColMax();

	// calculate last tile to render, based on current offset
	_lastTile = _offset;
	_lastTile.modInv().modAdd(_screenWidth + maxOffset, _screenHeight + maxOffset);
	_lastTile = _firstTile + (_lastTile / _tileSize);
	if (colmax < _lastTile._x) _lastTile._x = colmax;
	if (rowmax < _lastTile._y) _lastTile._y = rowmax;

	_render = true;
}


void ScreenCtl::updateTimers() {
	if (frame_t.check()) {
		_render = true;
		_animation_frame++;
		if (_animation_frame > 47) _animation_frame = 0;
		frame_t.relaunch();
		_button->update();
	}
	_menu->update();
}

shared_ptr<ScreenArea> ScreenCtl::processAction(const Point& pos) {
	return _buffer.getElement(pos);
}

void ScreenCtl::displayOptions(Point objPos, const shared_ptr<ObjectActionArea> options) {
	hideOptions();
	convertMapCoords(objPos).modAdd(0,-20);
	_menu->setPos(objPos);
	for (auto it = options->_acts.begin(); it != options->_acts.end(); it++) {
		auto newOption = make_shared<UIButton>(objPos, nullptr, Point(100, 25), *it, nullptr,
			nullptr, _font, std::string("Item ") + std::to_string(*it), true, false);

		_options.push_back(newOption);
		_menu->addOption(newOption);
	}	
}

void ScreenCtl::hideOptions() {
	_menu->reset();
	_options.clear();
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
	return convertCoords(x, y)._x;
}

int ScreenCtl::convertScreenY(int x, int y) {
	return convertCoords(x, y)._y;
}

Point ScreenCtl::convertCoords(int x, int y) {
	Point coord(x - _tileSize._x, y);
	coord -= _screenOffset;
	return ((coord.toMap() - _offset) * TILE_MASK) / _tileSize + _firstTile * TILE_MASK;
}

Point ScreenCtl::convertCoords(const Point& pos) {
	return convertCoords(pos._x, pos._y);
}

Point& ScreenCtl::convertMapCoords(Point& coord) {
	coord = (coord - _firstTile * TILE_MASK) * _tileSize / TILE_MASK + _offset;
	coord = coord.toIso().modAdd(_tileSize._x, 0) + _screenOffset;
	return coord;
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
		update();
	}
}

void ScreenCtl::zoomOut() {
	if (_zoom > 0.5) {
		_zoom -= 0.1;
		_tileWidth = TD_TILESIZE_X * _zoom;
		_tileHeight = TD_TILESIZE_Y * _zoom;
		update();
	}
}