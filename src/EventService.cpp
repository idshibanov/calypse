#include <stdlib.h>

#include "EventService.h"
#include "Map.h"

EventService::EventService(shared_ptr<ResourceCtl> res, shared_ptr<LocalMap> map, 
	                       shared_ptr<AStarSearch> pf, shared_ptr<AppState> state) {
	_res = res;
	_map = map;
	_pFinder = pf;
	_state = state;
	srand(time(NULL));
}

EventService::~EventService() {

}

void EventService::process(shared_ptr<ActionInfo> act) {
	auto obj = _map->getObject(_state->_selectedObject);
	process(obj, act);
}

void EventService::process(shared_ptr<MapObject> obj, shared_ptr<ActionInfo> act) {
	auto actor = _map->getPrimaryActor();
	Rect objArea(obj->getPos(), _res->getObjectInfo(obj->getType())->mapSize());
	Point target = _pFinder->findAdjacent(actor->getPos(), objArea);

	if (target._x >= 0 && target._y >= 0) {
		auto act1 = make_shared<MoveAction>(_res->getActionInfo("move"), _res, actor, target, _pFinder);
		if (act->type() == ACTION_ABS_CARRY && _res->getObjectInfo(obj->getType())->liftable()) {
			auto act2 = make_shared<ObjectAction>(act, _res, actor, obj, _map);
			act1->chainAction(act2);
		} else {
			auto act2 = make_shared<ObjectAction>(act, _res, actor, obj, _map);
			act1->chainAction(act2);
		}
		actor->setAction(act1);
	}
}

void EventService::spawnAction(int actID) {
	auto actor = std::dynamic_pointer_cast<SmartActor>(_map->getObject(actID));
	if (actor) {
		Point pos = actor->getPos();
		int modX = rand() % 11 - 5;
		int modY = rand() % 11 - 5;
		pos.modAdd(modX * TILE_MASK, modY * TILE_MASK);

		auto act = make_shared<MoveAction>(_res->getActionInfo("move"), _res, actor, pos, _pFinder);
		actor->setAction(act);
	}
}