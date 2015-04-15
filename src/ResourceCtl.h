#pragma once
#include <map>

#include "ObjectInfo.h"
#include "Sprite.h"

// Resource controller that will eventually handle gradual loading
// Configuration files (XML?) are planned

class ResourceCtl {
	std::map<int, shared_ptr<ObjectInfo>> _info;
	std::map<int, shared_ptr<Sprite>> _sprites;
public:
	ResourceCtl();
	~ResourceCtl();
	void loadSprites();
	shared_ptr<ObjectInfo> getObjectInfo(int type);
	shared_ptr<Sprite> getSprite(int id);
};