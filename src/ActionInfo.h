#pragma once
#include <string>

#include "ActionType.h"

class ActionInfo {
	ActionAbstractType _type;
	std::string _name;
	int _cycles;
	int _steps;

	int _itemParam;
	int _objParam;
	int _quantity;
public:
	ActionInfo(ActionAbstractType, const std::string&, int, int, int = -1, int = -1, int = -1);
	ActionInfo(const ActionInfo&);
	virtual ~ActionInfo();
	ActionInfo& operator=(const ActionInfo&);
	int type() const;
	int cycles() const;
	int steps() const;
	std::string name() const;
	int itemParam() const;
	int objParam() const;
	int quantity() const;
};
