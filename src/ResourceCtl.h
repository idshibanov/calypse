#pragma once
#include <map>

#include "ConfigCtl.h"
#include "ObjectInfo.h"
#include "ItemInfo.h"
#include "ActionTemplate.h"
#include "ActionInfo.h"
#include "Sprite.h"

#define INVENTORY_ICON_SIZE 48
#define ACTION_NONE -1

// Resource controller that will eventually handle gradual loading
// Configuration files (JSON) are being implemented

// toResolve map - names and ids
// resolveID function that will be fired when right object added
// alternative - value map, updated when loaded
// Info objects are pointed to map (record pushed/ptr passed when created)

class ResPromise {
	ResPromise();
public:
	std::string _key;
	const std::map<std::string, int>& _lookup;
	int& _toResolve;
	ResPromise(std::string k, const std::map<std::string, int>& l, int& r);
};

class ResourceCtl {
	shared_ptr<ConfigCtl> _conf;
	std::map<std::string, int> _objectLookup;
	std::map<std::string, int> _itemLookup;
	std::map<std::string, int> _spriteLookup;
	std::map<std::string, int> _actionLookup;
	std::map<std::string, shared_ptr<ActionTemplate>> _actionTemplates;
	int _lastAct;

	std::map<int, shared_ptr<ObjectInfo>> _info;
	std::map<int, shared_ptr<ItemInfo>> _itemInfo;
	std::map<int, shared_ptr<ActionInfo>> _actionInfo;


	std::map<int, shared_ptr<Sprite>> _sprites;
	std::map<int, shared_ptr<SpriteText>> _arialFonts;

	std::vector<ResPromise> _promises;
	void _makePromise(std::string key, const std::map<std::string, int>& lookup, int& val);
	void _resolvePromises();

	bool _confRecordExists(ConfigCategory cat, const std::string& param, const std::string& key) const;

	void _loadObjectRecords();
	void _loadItemRecords();
	void _loadSprites();
	void _loadActionTemplates();
	void _loadDefaultActions();
	int _loadAction(shared_ptr<JsonObject> actRecord);
	void _loadActiveAreas(shared_ptr<ObjectInfo> info, shared_ptr<JsonObject> actArea);
public:
	ResourceCtl(shared_ptr<ConfigCtl> conf);
	~ResourceCtl();
	void loadResources();
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
	int getActionID(const char* name) const;
	shared_ptr<ActionInfo> getActionInfo(const char* name) const;
	shared_ptr<ActionInfo> getActionInfo(int id) const;
	ActionAbstractType matchActionAbstract(std::string& name) const;
	std::string getStatName(StatScoreID id) const;
	std::string getSkillName(SkillScoreID id) const;
	std::string getItemName(int id) const;
};