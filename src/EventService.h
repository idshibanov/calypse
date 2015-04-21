#pragma once
#include "AppState.h"
#include "ResourceCtl.h"
#include "Action.h"

class LocalMap;
class AStarSearch;

class EventService {
	shared_ptr<ResourceCtl> _res;
	shared_ptr<LocalMap> _map;
	shared_ptr<AStarSearch> _pFinder;
	shared_ptr<AppState> _state;
public:
	EventService(shared_ptr<ResourceCtl>, shared_ptr<LocalMap>, shared_ptr<AStarSearch>, shared_ptr<AppState>);
	~EventService();
	shared_ptr<Action> createAction();
	void process(ActionType id);
	void process(shared_ptr<MapObject> obj);
	void spawnAction(int actID);
};