#pragma once
#include <string>

#include "ActionType.h"

class ActionInfo {
	ActionAbstractType _type;
	std::string _name;
	int _timer;
	int _cycles;
public:
	ActionInfo(ActionAbstractType, const std::string&, int, int);
	ActionInfo(const ActionInfo&);
	virtual ~ActionInfo();
	ActionInfo& operator=(const ActionInfo&);
	int type() const;
	int timer() const;
	int cycles() const;
	std::string name() const;
};
