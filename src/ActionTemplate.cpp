#include "ActionTemplate.h"


ActionTemplate::ActionTemplate(ActionAbstractType type, const std::string& name, int timer, 
                               int cycles, std::vector<std::string> params) {
	_type = type;
	_name = name;
	_timer = timer;
	_cycles = cycles;
	_params = std::move(params); // TODO: check performance
}

ActionTemplate::ActionTemplate(const ActionTemplate& rhs) {
	_type = rhs._type;
	_name = rhs._name;
	_timer = rhs._timer;
	_cycles = rhs._cycles;
	_params = rhs._params;
}

ActionTemplate& ActionTemplate::operator= (const ActionTemplate& rhs) {
	_type = rhs._type;
	_name = rhs._name;
	_timer = rhs._timer;
	_cycles = rhs._cycles;
	_params = rhs._params;
	return *this;
}

ActionTemplate::~ActionTemplate() {

}

int ActionTemplate::type() const {
	return _type;
}

int ActionTemplate::timer() const {
	return _timer;
}

int ActionTemplate::cycles() const {
	return _cycles;
}

std::string ActionTemplate::name() const {
	return _name;
}

const std::vector<std::string>& ActionTemplate::params() const {
	return _params;
}