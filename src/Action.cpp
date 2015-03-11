#include "Object.h"
#include "Action.h"

Action::Action(ActionType type, weak_ptr<Actor> actor, int cycles, int steps, const Point& pos, weak_ptr<AStarSearch> pf)
               : _timer(cycles), _state(steps) {
	_type = type;
	_target = pos - (pos % 2);
	_actor = actor;

	_pf = pf;
	auto finder = _pf.lock();
	auto act = _actor.lock();
	if (finder && act) {
		_path = _pf.lock()->searchPath(act->getXPos(), act->getYPos(), _target._x, _target._y);
	}
}

Action::~Action() {

}

Action::Action (const Action& act) {
	_type = act._type;
	_actor = act._actor;
	_target = act._target;
	_timer = act._timer;
	_state = act._state;
	_path = act._path;
	_pf = act._pf;
}

Action& Action::operator= (const Action& act) {
	_type = act._type;
	_actor = act._actor;
	_target = act._target;
	_timer = act._timer;
	_state = act._state;
	_path = act._path;
	_pf = act._pf;
	return *this;
}

void Action::stop() {
	_timer.setTicks(0);
}

bool Action::isActive() const {
	return _timer.isActive();
}

int Action::getState() const {
	return _state.getProgress();
}

bool Action::update() {
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
				Point next(nextNode._mapX, nextNode._mapY);
				mod = (next - (act->getPos() / TILE_MASK)) * SUBTILE_MASK;
				act->move(mod);

				if (act->getXPos() / TILE_MASK == nextNode._mapX && act->getYPos() / TILE_MASK == nextNode._mapY) {
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

