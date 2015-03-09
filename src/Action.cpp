#include "Action.h"

Action::Action(ActionType type, int cycles, int steps, const Point& pos, weak_ptr<MapObject> target) 
               : _timer(cycles) {
	_type = type;
	_target = pos;
}

Action::~Action() {

}

Action::Action (const Action& act) {
	_type = act._type;
	_target = act._target;
	_timer = act._timer;
}

Action& Action::operator= (const Action& act) {
	_type = act._type;
	_target = act._target;
	_timer = act._timer;
}

void Action::stop() {
	_timer.setTicks(0);
}

bool Action::isActive() const {
	return _timer.isActive();
}

bool Action::update() {
	if (_timer.check()) {
		return true;
	}
	return false;
}

