#pragma once
#include "Point.h"

// Will be extended soon with derived classes.
// Basically works as a resource entry, holding all now-hardcoded parameters.

class ObjectInfo {
	// Object type ID, for comparisson
	int _type;

	// Object map size (full precision, x10)
	Point _mapSize;

	// Sprite size (for the spritesheet)
	Point _sprSize;

	// Rendering offset
	Point _offset;

	// ID of the spritesheet
	int _spriteID;

	// Number of frames (0 = single, more = spritesheet)
	int _frames;

	// Object can be lifted
	bool _liftable;
public:
	ObjectInfo(int, const Point&, const Point&, const Point&, int, int, bool);
	ObjectInfo(ObjectInfo& rhs);
	ObjectInfo& operator=(ObjectInfo& rhs);
	~ObjectInfo();
	int type() const;
	Point mapSize() const;
	Point sprSize() const;
	Point offset() const;
	int spriteID() const;
	int frames() const;
	bool liftable() const;
};