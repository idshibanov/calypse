#include "ActionInfo.h"


ActionInfo::ActionInfo(ActionAbstractType type, const std::string& name, int cycles, int steps, 
                       int item, int obj, int quantity) {
	_type = type;
	_name = name;
	_cycles = cycles;
	_steps = steps;
	_itemParam = item;
	_objParam = obj;
	_quantity = quantity;
}

ActionInfo::ActionInfo(const ActionInfo& rhs) {
	_type = rhs._type;
	_name = rhs._name;
	_cycles = rhs._cycles;
	_steps = rhs._steps;
}

ActionInfo& ActionInfo::operator= (const ActionInfo& rhs) {
	_type = rhs._type;
	_name = rhs._name;
	_cycles = rhs._cycles;
	_steps = rhs._steps;
	return *this;
}

ActionInfo::~ActionInfo() {

}

int ActionInfo::type() const {
	return _type;
}

int ActionInfo::cycles() const {
	return _cycles;
}

int ActionInfo::steps() const {
	return _steps;
}

std::string ActionInfo::name() const {
	return _name;
}

int ActionInfo::itemParam() const {
	return _itemParam;
}

int ActionInfo::objParam() const {
	return _objParam;
}

int ActionInfo::quantity() const {
	return _quantity;
}

