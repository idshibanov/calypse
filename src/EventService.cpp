#include <stdlib.h>

#include "EventService.h"
#include "Action.h"
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

void EventService::spawnAction(int actID) {
	auto actor = std::dynamic_pointer_cast<SmartActor>(_map->getObject(actID));
	if (actor) {
		Point pos = actor->getPos();
		int modX = rand() % 11 - 5;
		int modY = rand() % 11 - 5;
		pos.modAdd(modX * TILE_MASK, modY * TILE_MASK);

		auto act = make_shared<MoveAction>(ACTION_MOVE, _res, actor, 8, 8, pos, _pFinder);
		actor->setAction(act);
	}
}