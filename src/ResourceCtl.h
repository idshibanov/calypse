#pragma once
#include <map>

#include "ObjectInfo.h"
#include "Sprite.h"

// Resource controller that will eventually handle gradual loading
// Configuration files (XML?) are planned

class ResourceCtl {
	map<int, shared_ptr<ObjectInfo>> _info;
	map<int, shared_ptr<Sprite>> _sprites;
public:
	ResourceCtl();
	~ResourceCtl();
	void loadSprites();
	weak_ptr<ObjectInfo> getObjectInfo(int type);
	shared_ptr<Sprite> getSprite(int id);
};