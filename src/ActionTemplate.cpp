#include "ActionTemplate.h"


ActionTemplate::ActionTemplate(ActionAbstractType type, const std::string& name, int cycles, 
                               int steps, std::vector<std::string> params) {
	_type = type;
	_name = name;
	_cycles = cycles;
	_steps = steps;
	_params = std::move(params); // TODO: check performance
}

ActionTemplate::ActionTemplate(const ActionTemplate& rhs) {
	_type = rhs._type;
	_name = rhs._name;
	_cycles = rhs._cycles;
	_steps = rhs._steps;
	_params = rhs._params;
}

ActionTemplate& ActionTemplate::operator= (const ActionTemplate& rhs) {
	_type = rhs._type;
	_name = rhs._name;
	_cycles = rhs._cycles;
	_steps = rhs._steps;
	_params = rhs._params;
	return *this;
}

ActionTemplate::~ActionTemplate() {

}

int ActionTemplate::type() const {
	return _type;
}

int ActionTemplate::steps() const {
	return _steps;
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