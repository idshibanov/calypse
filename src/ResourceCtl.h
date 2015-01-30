#pragma once
#include <unordered_map>

#include "ObjectInfo.h"
#include "Sprite.h"

// Resource controller that will eventually handle gradual loading
// Configuration files (XML?) are planned

class ResourceCtl {
	unordered_map<short, ObjectInfo> _info;
	vector<shared_ptr<Sprite>> _sprites;
public:
	ResourceCtl();
	~ResourceCtl();
	unordered_map<short, ObjectInfo>::iterator getObjectInfo(int type);
};