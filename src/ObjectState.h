#pragma once

class ActorState {
	int _exp;
	int _hp;
	int _mp;
	int _hunger;

	bool _run;

	int _str;
	int _con;
	int _dex;
	int _int;
public:
	ActorState();
	~ActorState();
};