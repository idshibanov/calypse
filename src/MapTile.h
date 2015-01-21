#pragma once
#include "Main.h"

class MapTile {
	char _type;
	char _sprite;
	uint32_t _properties;
public:
	MapTile(char type);
	MapTile(char type, char sprite);
	~MapTile();
	char getType() const;
	void setType(char type);
	bool getSubtile(unsigned pos);
	bool getSubtile(unsigned x, unsigned y);
	void setSubtile(unsigned pos, bool bit = true);
};