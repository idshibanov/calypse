#include "ActionInfo.h"


ActionInfo::ActionInfo(ActionAbstractType type, const std::string& name, int timer, int cycles, 
                       int item, int obj, int quantity) {
	_type = type;
	_name = name;
	_timer = timer;
	_cycles = cycles;
	_itemParam = item;
	_objParam = obj;
	_quantity = quantity;
}

ActionInfo::ActionInfo(const ActionInfo& rhs) {
	_type = rhs._type;
	_name = rhs._name;
	_timer = rhs._timer;
	_cycles = rhs._cycles;
}

ActionInfo& ActionInfo::operator= (const ActionInfo& rhs) {
	_type = rhs._type;
	_name = rhs._name;
	_timer = rhs._timer;
	_cycles = rhs._cycles;
	return *this;
}

ActionInfo::~ActionInfo() {

}

int ActionInfo::type() const {
	return _type;
}

int ActionInfo::timer() const {
	return _timer;
}

int ActionInfo::cycles() const {
	return _cycles;
}

std::string ActionInfo::name() const {
	return _name;
}
