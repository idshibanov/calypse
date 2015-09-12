#pragma once
#include <string>
#include <vector>

#include "ActionType.h"

class ActionTemplate {
	ActionAbstractType _type;
	std::string _name;
	int _cycles;
	int _steps;
	std::vector<std::string> _params;
public:
	ActionTemplate(ActionAbstractType, const std::string&, int, int, std::vector<std::string>);
	ActionTemplate(const ActionTemplate&);
	virtual ~ActionTemplate();
	ActionTemplate& operator=(const ActionTemplate&);
	ActionAbstractType type() const;
	int cycles() const;
	int steps() const;
	std::string name() const;
	const std::vector<std::string>& params() const;
};
