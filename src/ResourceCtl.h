#pragma once
#include <map>

#include "ConfigCtl.h"
#include "ObjectInfo.h"
#include "ItemInfo.h"
#include "Sprite.h"

#define INVENTORY_ICON_SIZE 48

// Resource controller that will eventually handle gradual loading
// Configuration files (JSON) are being implemented

class ResourceCtl {
	shared_ptr<ConfigCtl> _conf;
	std::map<std::string, int> _actionLookup;
	std::map<std::string, int> _objectLookup;
	std::map<std::string, int> _itemLookup;
	std::map<std::string, int> _spriteLookup;

	std::map<int, shared_ptr<ObjectInfo>> _info;
	std::map<int, shared_ptr<ItemInfo>> _itemInfo;

	std::map<int, shared_ptr<Sprite>> _sprites;
	std::map<int, shared_ptr<SpriteText>> _arialFonts;

	void loadObjectRecords();
	void loadItemRecords();
	void loadActiveAreas(shared_ptr<ObjectInfo> info, shared_ptr<JsonObject> actArea);
public:
	ResourceCtl(shared_ptr<ConfigCtl> conf);
	~ResourceCtl();
	void loadResources();
	void loadSprites();
	int getObjectID(const char* name) const;
	shared_ptr<ObjectInfo> getObjectInfo(const char* name) const;
	shared_ptr<ObjectInfo> getObjectInfo(int type) const;
	int getItemCount() const;
	int getItemID(const char* name) const;
	shared_ptr<ItemInfo> getItemInfo(const char* name) const;
	shared_ptr<ItemInfo> getItemInfo(int type) const;
	int getSpriteID(const char* name) const;
	shared_ptr<Sprite> getSprite(int id) const;
	shared_ptr<Sprite> getSprite(const char* name) const;
	shared_ptr<SpriteSheet> getSpriteSheet(int id) const;
	shared_ptr<SpriteSheet> getSpriteSheet(const char* name) const;
	shared_ptr<SpriteText> getFont(int size);
	void loadActions();
	int getActionID(const char* name) const;
	std::string getActionName (int id) const;
	std::string getStatName(StatScoreID id) const;
	std::string getSkillName(SkillScoreID id) const;
	std::string getItemName(int id) const;
};