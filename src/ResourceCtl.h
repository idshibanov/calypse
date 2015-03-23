#pragma once
#include <map>

#include "ObjectInfo.h"
#include "Sprite.h"

// Resource controller that will eventually handle gradual loading
// Configuration files (XML?) are planned

class ResourceCtl {
	std::map<int, std::shared_ptr<ObjectInfo>> _info;
	std::map<int, std::shared_ptr<Sprite>> _sprites;
public:
	ResourceCtl();
	~ResourceCtl();
	void loadSprites();
	std::weak_ptr<ObjectInfo> getObjectInfo(int type);
	std::shared_ptr<Sprite> getSprite(int id);
};