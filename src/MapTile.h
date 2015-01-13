#pragma once
#include "Main.h"

class MapTile {
	char _type;
	char _sprite;
public:
	MapTile(char type);
	MapTile(char type, char sprite);
	~MapTile();
	char getType() const;
	void setType(char type);
};