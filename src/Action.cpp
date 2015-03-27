#include "Map.h"
#include "Object.h"
#include "Action.h"

Action::Action(ActionType type, std::weak_ptr<Actor> actor, int cycles, int steps)
               : _timer(cycles), _state(steps) {
	_type = type;
	_actor = actor;
	// don't start the timer until action started
	_timer.stop();
}

Action::~Action() {

}

Action::Action (const Action& act) {
	_type = act._type;
	_actor = act._actor;
	_timer = act._timer;
	_state = act._state;
	_next = act._next;
}

Action& Action::operator= (const Action& act) {
	_type = act._type;
	_actor = act._actor;
	_timer = act._timer;
	_state = act._state;
	_next = act._next;
	return *this;
}

void Action::chainAction(std::shared_ptr<Action> act) {
	if (_next) {
		_next->stop();
	}
	_next = act;
}

std::shared_ptr<Action> Action::getNext() const {
	return _next;
}

void Action::stop() {
	_timer.setTicks(0);
}

int Action::getState() const {
	return _state.getProgress();
}

ActionType Action::getType() const {
	return _type;
}



MoveAction::MoveAction(ActionType type, std::weak_ptr<Actor> actor, int cycles, int steps,
	                   const Point& pos, std::weak_ptr<AStarSearch> pf)
                       : Action(type, actor, cycles, steps) {
	_target = pos - (pos % 2);
	_pf = pf;
}

MoveAction::MoveAction(const MoveAction& act) : Action(act) {
	_target = act._target;
	_path = act._path;
	_pf = act._pf;
}

MoveAction::~MoveAction() {

}

MoveAction& MoveAction::operator= (const MoveAction& act) {
	Action::operator=(act);
	_target = act._target;
	_path = act._path;
	_pf = act._pf;
	return *this;
}

bool MoveAction::isActive() const {
	return !_path.empty();
}

void MoveAction::start() {
	auto finder = _pf.lock();
	auto act = _actor.lock();
	if (finder && act) {
		_path = finder->searchPath(act->getPos(), _target);
		_timer.relaunch();
	}
}

bool MoveAction::update() {
	auto act = _actor.lock();
	if (!_path.empty() && act) {
		if (_timer.check()) {
			bool lastTile = _path.size() == 1;
			Point mod;

			if (lastTile) {
				mod = _target - act->getPos();
				mod = mod.limit(SUBTILE_MASK);
				act->move(mod);

				if (act->getPos() == _target) {
					// action is done, we may return prematurely
					_path.erase(_path.begin());
					_state.stop();
					return true;
				}
			} else {
				AStarNode& nextNode = _path.front();
				Point next = nextNode._pos;
				mod = (next - (act->getPos() / TILE_MASK)) * SUBTILE_MASK;
				act->move(mod);

				if (act->getXPos() / TILE_MASK == nextNode._pos._x && act->getYPos() / TILE_MASK == nextNode._pos._y) {
					_path.erase(_path.begin());
				}
			}
			_timer.relaunch();
			if (_state.check()) {
				_state.relaunch();
			}
			return true;
		}
	}
	return false;
}



ObjectAction::ObjectAction(ActionType type, std::weak_ptr<Actor> actor, int cycles, int steps,
	                       std::weak_ptr<MapObject> obj, std::weak_ptr<LocalMap> map)
	                       : Action(type, actor, cycles, steps) {
	_target = obj;
	_map = map;
}

ObjectAction::ObjectAction(const ObjectAction& act) : Action(act) {
	_target = act._target;
	_map = act._map;
}

ObjectAction::~ObjectAction() {

}

ObjectAction& ObjectAction::operator= (const ObjectAction& act) {
	Action::operator=(act);
	_target = act._target;
	_map = act._map;
	return *this;
}

bool ObjectAction::isActive() const {
	return _timer.isActive();
}

void ObjectAction::start() {
	_timer.relaunch();
}

bool ObjectAction::update() {
	if (_timer.check()) {
		//auto obj = make_shared<MapObject>(Point(12, 12), 1);
		//return _map.lock()->addObject(obj);
		if (_state.check()) {
			if (!_map.expired() && !_target.expired()) {
				if (_type == ACTION_CUT) {
					return _map.lock()->toggleObject(_target.lock()->getPos());
				} else if (_type == ACTION_DRAG && !_actor.expired()) {
					_actor.lock()->pickUp(_target);
					_target.lock()->dragged(true);
					return true;
				}
			}
		}
		_timer.relaunch();
	}
	return false;
}




PointAction::PointAction(ActionType type, std::weak_ptr<Actor> actor, int cycles, int steps, const Point& pos)
	: Action(type, actor, cycles, steps) {
	_targetPos = pos;
}

PointAction::PointAction(const PointAction& act) : Action(act) {
	_targetPos = act._targetPos;
}

PointAction::~PointAction() {

}

PointAction& PointAction::operator= (const PointAction& act) {
	Action::operator=(act);
	_targetPos = act._targetPos;
	return *this;
}

bool PointAction::isActive() const {
	return _state.isActive();
}

void PointAction::start() {
	_timer.relaunch();
	_state.relaunch();
}

bool PointAction::update() {
	if (_timer.check()) {
		if (_state.check()) {
			if (!_actor.expired()) {
				if (_type == ACTION_DROP) {
					_actor.lock()->drop(_targetPos);
					return true;
				}
			}
		}
		_timer.relaunch();
	}
	return false;
}
