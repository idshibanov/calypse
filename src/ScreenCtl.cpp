#include <iomanip>
#include <sstream>
#include <allegro5/allegro_primitives.h>

#include "ScreenCtl.h"
#include "Object.h"

ScreenCtl::ScreenCtl (shared_ptr<ResourceCtl> res, shared_ptr<LocalMap> map, shared_ptr<Camera> cam, 
	                  shared_ptr<Mouse> mouse, shared_ptr<AppState> stats)
					  : frame_t(ANIMATION_TICKS) {

	_tileWidth = TD_TILESIZE_X;
	_tileHeight = TD_TILESIZE_Y;
	_tileSize.set(_tileWidth, _tileHeight);

	_res = res;
	_map = map;
	_cam = cam;
	_mouse = mouse;
	_state = stats;

	_font = _res->getFont(12);
	_lastTimestamp = _state->_appTime;

	_zoom = 1.0;
	_animation_speed = 100;
	_animation_frame = 0;

	_menu = make_shared<CarouselMenu>(Point(300, 200), Point(100, 100), nullptr);

	_render = true;
}

ScreenCtl::~ScreenCtl() {

}

void ScreenCtl::reloadScreen() {
	_render = true;
	_frames.clear();

	if (_state->_curScreen && _state->_curScreen->isMapScreen()) {

		_frames.push_back(make_shared<ObjectInfoFrame>(Point(300, 200), Point(200, 90), _res,
			std::string("Character Sheet"), _map->getPrimaryActor()->getState()));

		_frames.push_back(make_shared<ContainerFrame>(Point(300, 200), _res, std::string("Inventory"),
			_map->getPrimaryActor()->getState()->getInventory()));

	}
}


bool ScreenCtl::draw() {
	bool retval = false;
	if (_state->_curScreen  && (_render || _lastTimestamp < _state->_appTime)) {
		_buffer.reset();
		ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);

		_state->_curScreen->draw(_state->_resolution);
		drawMap();

		auto frames = _state->_curScreen->getFrames();
		for (auto fr : frames) {
			if (fr->isVisible()) {
				fr->draw();
				_buffer.setElement(fr);
			}
		}


		if (_state->_drawUIAreas) {
			auto bufferItems = _buffer.getAllAreas();
			for (auto it : bufferItems) {
				al_draw_rectangle(it->getPos()._x, it->getPos()._y, it->getMax()._x, it->getMax()._y, al_map_rgb(238, 33, 238), 2);
			}
		}
		
		if (_state->_drawStats) {
			std::string timeSTR("App time: " + std::to_string(_state->_appTime));
			std::string cpsSTR("Cycles per second: " + std::to_string(_state->_CPS));
			std::string fpsSTR("Frames per second: " + std::to_string(_state->_FPS));
			std::string spdSTR("Animation speed: " + std::to_string(static_cast<int>(_animation_speed)) + "%");
			_font->draw(timeSTR, Point(5, 5), color);
			_font->draw(cpsSTR, Point(5, 30), color);
			_font->draw(fpsSTR, Point(5, 55), color);
			_font->draw(spdSTR, Point(5, 80), color);
		}

		// Almost last: mouse cursor and selected item
		if (_state->_selectedItem) {
			auto itInfo = _res->getItemInfo(_state->_selectedItem->getType());
			auto itSpr = _res->getSpriteSheet(itInfo->spriteID());
			if (itSpr) {
				itSpr->draw(_mouse->getPos(), itInfo->frame());
			} else {
				cout << "ERROR: could not find item sprite #" << itInfo->spriteID() << endl;
			}
		}

		auto cur = std::dynamic_pointer_cast<SpriteSheet>(_res->getSprite("cursor"));
		cur->draw(_mouse->getPos(), _mouse->getSprite());

		_lastTimestamp = _state->_appTime;
		_render = false;
		retval = true;
	}

	return retval;
}


void ScreenCtl::drawMap() {
	if (_state->_curScreen->isMapScreen()) {
		ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);
		ALLEGRO_COLOR obj_color = al_map_rgb(155, 155, 255);
		Point isoTileSize(_tileSize._x * 2, _tileSize._y);

		// Map tiles
		auto grass = _res->getSprite("grass").get();
		//al_hold_bitmap_drawing(true);
		for (int row = _firstTile._y; row < _lastTile._y; row++) {
			for (int col = _firstTile._x; col < _lastTile._x; col++) {
				Point coord(col, row);
				coord = (coord - _firstTile) * _tileSize + _offset;
				coord = coord.toIso() + _screenOffset;

				// loop drawing sub bitmaps must be the same parent
				grass->drawScaled(coord, isoTileSize);

				if (_state->_drawCoords) {
					std::string tileCoords(std::to_string(col) + ", " + std::to_string(row));
					_font->draw(tileCoords, coord.add(24, 8), color);
				}
			}
		}
		//al_hold_bitmap_drawing(false);

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



		auto groundItems = _map->getItems(_firstTile, _lastTile);
		double itemZoom = _zoom / GROUND_ITEMS_FACTOR;

		for (auto item : groundItems) {
			Point coord = item.first;
			convertMapCoords(coord);

			auto itemInfo = _res->getItemInfo(item.second->getType());

			if (itemInfo != nullptr) {
				Point itemSize = itemInfo->sprSize() * itemZoom;
				Point itemOffset = itemInfo->offset() * itemZoom;
				coord -= itemOffset;

				auto itemSprite = _res->getSprite(itemInfo->spriteID());
				auto itemSpriteSheet = std::dynamic_pointer_cast<SpriteSheet>(itemSprite);
				itemSpriteSheet->drawScaled(coord, itemInfo->frame(), itemZoom);

				_buffer.setElement(make_shared<ItemArea>(coord, itemSize, item.second, itemSpriteSheet));
			}
		}


		shared_ptr<Actor> _actor = _map->getPrimaryActor();
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

				auto objSprite = _res->getSprite(objInfo->spriteID());
				if (objInfo->frames() == 0) {
					objSprite->drawScaled(coord, objSize);

					_buffer.setElement(make_shared<ObjectArea>(coord, objSize, obj.second, objSprite, objInfo));
				} else {
					auto objSpriteSheet = std::dynamic_pointer_cast<SpriteSheet>(objSprite);
					objSpriteSheet->drawScaled(coord, obj.second->getSprite(), _zoom);

					_buffer.setElement(make_shared<ObjectArea>(coord, objSize, obj.second, objSpriteSheet, objInfo));
				}

				//string tileCoords(to_string(obj.second->getXPos()) + ", " + to_string(obj.second->getYPos()));
				//_font->draw(tileCoords, coord.add(12, 30), color);
				//_font->draw(to_string(obj.first), coord.add(12, 30), color);
				//_font->draw(std::to_string(obj.second->getID()), coord.add(12, 30), obj_color);
			}
		}

		// Selected object (craft suggestion)
		if (_state->_selectedAction != -1) {
			if (_state->_selectedAction == _res->getActionID("craftReet")) {
				drawSelectedObject(_res->getObjectInfo("reet"));
			} else if (_state->_selectedAction == _res->getActionID("craftFire")) {
				drawSelectedObject(_res->getObjectInfo("fire"));
			}
		}


		// UI rendering

		if (_actor->isWorking()) {
			Point coord = convertMapCoords(_actor->getPos());
			_font->draw(std::to_string(_actor->getProgress()), coord.modAdd(25, 0) + _res->getObjectInfo(_actor->getType())->offset(), color);
		}

		for (auto button : _options) {
			if (button->isVisible()) {
				button->draw();
				_buffer.setElement(button);
			}
		}

		for (auto fr : _frames) {
			if (fr->isVisible()) {
				fr->draw();
				_buffer.setElement(fr);
			}
		}
	}
}


void ScreenCtl::redraw(bool cameraMoved) {
	if (cameraMoved) update();

	_render = true;
}


void ScreenCtl::update() {
	if (_state->_curScreen->isMapScreen()) {

		// camera position
		Point camPos = _cam->getPos();
		// map rendering offset, based on camera position
		_offset.set(0, 0);

		// offset after which we shift the tiles
		int maxOffset = (_state->_resolution._x / 2 + _state->_resolution._y) / 2;

		_screenOffset.set(_state->_resolution._x / 2 - _tileWidth, _state->_resolution._y / 2);
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
		_lastTile.modInv().modAdd(_state->_resolution._x + maxOffset, _state->_resolution._y + maxOffset);
		_lastTile = _firstTile + (_lastTile / _tileSize);
		if (colmax < _lastTile._x) _lastTile._x = colmax;
		if (rowmax < _lastTile._y) _lastTile._y = rowmax;
	}
	_render = true;
}


void ScreenCtl::updateTimers() {
	if (frame_t.check()) {
		_render = true;
		_animation_frame++;
		if (_animation_frame > 47) _animation_frame = 0;
		frame_t.relaunch();
	}
	if (_menu) _menu->update();
}


void ScreenCtl::drawSelectedObject(shared_ptr<ObjectInfo> obj) {
	Point actCoord = convertCoords(_mouse->getPos()).modDiv(SUBTILE_MASK).modMul(SUBTILE_MASK);

	if (actCoord > 0) {
		actCoord = (actCoord * _tileSize) / TILE_MASK + _offset - (_firstTile * _tileSize);
		actCoord = actCoord.toIso() + _screenOffset;

		auto reetSprite = std::dynamic_pointer_cast<SpriteSheet>(_res->getSprite(obj->spriteID()));
		actCoord += obj->offset() * _zoom;

		reetSprite->drawTinted(actCoord, 0, _zoom, al_map_rgba_f(1, 1, 1, 0.7));
	}
}


shared_ptr<ScreenArea> ScreenCtl::processAction(const Point& pos) {
	return _buffer.getElement(pos);
}


void ScreenCtl::displayOptions(Point objPos, const shared_ptr<ObjectActionArea> options) {
	//hideOptions();
	_menu->reset();
	_options.clear();
	convertMapCoords(objPos).modAdd(0,-20);
	_menu->setPos(objPos);
	for (auto it = options->_acts.begin(); it != options->_acts.end(); it++) {
		auto newOption = make_shared<UIButton>(objPos, nullptr, *it, _font, _res->getActionInfo(*it)->name(), true, false);
		newOption->setZlevel(90);

		_options.push_back(newOption);
		_menu->addOption(newOption);
	}	
}


void ScreenCtl::hideOptions() {
	_menu->closeAnimation();
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

void ScreenCtl::toggleInfoScreen() {
	if (_frames[0]) {
		_frames[0]->setVisible(!_frames[0]->isVisible());
	}
}

void ScreenCtl::toggleInventory() {
	if (_frames[1]) {
		_frames[1]->setVisible(!_frames[1]->isVisible());
	}
}