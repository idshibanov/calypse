#pragma once
#include "Point.h"

// Will be extended soon with derived classes.
// Basically works as a resource entry, holding all now-hardcoded parameters.

class ObjectInfo {
public:
	Point _size;
	Point _offset;
	int _type;
	int _spriteID;
	bool _draggable;
	ObjectInfo(int, int, int, int, int, int, bool);
	ObjectInfo(ObjectInfo& rhs);
	ObjectInfo& operator=(ObjectInfo& rhs);
	~ObjectInfo();
};