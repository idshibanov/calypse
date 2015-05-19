#pragma once
#include <map>

#include "ObjectInfo.h"
#include "ItemInfo.h"
#include "Sprite.h"

#define INVENTORY_ICON_SIZE 48

// Resource controller that will eventually handle gradual loading
// Configuration files (XML?) are planned

class ResourceCtl {
	std::map<int, shared_ptr<ObjectInfo>> _info;
	std::map<int, shared_ptr<ItemInfo>> _itemInfo;
	std::map<int, shared_ptr<Sprite>> _sprites;
	std::map<int, shared_ptr<SpriteText>> _arialFonts;
public:
	ResourceCtl();
	~ResourceCtl();
	void loadSprites();
	shared_ptr<ObjectInfo> getObjectInfo(int type) const;
	shared_ptr<ItemInfo> getItemInfo(ItemType t) const;
	shared_ptr<Sprite> getSprite(int id) const;
	shared_ptr<SpriteSheet> getSpriteSheet(int id) const;
	shared_ptr<SpriteText> getFont(int size);
	std::string getActionName (ActionType id) const;
	std::string getStatName(StatScoreID id) const;
	std::string getSkillName(SkillScoreID id) const;
	std::string getItemName(ItemType id) const;
};