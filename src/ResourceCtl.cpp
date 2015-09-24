#include "ResourceCtl.h"

#include <future>

ResPromise::ResPromise(std::string k, const std::map<std::string, int>& l, int& r)
                      : _key(k), _lookup(l), _toResolve(r) {
}

ResourceCtl::ResourceCtl(shared_ptr<ConfigCtl> conf) {
	_conf = conf;
}

ResourceCtl::~ResourceCtl() {

}

bool ResourceCtl::_confRecordExists(ConfigCategory cat, const std::string& param, const std::string& key) const {
	auto ref = _conf->getCollection(cat, param);
	return ref.find(key) != ref.end();
}

void ResourceCtl::_makePromise(std::string key, const std::map<std::string, int>& lookup, int& val) {
	ResPromise p(key, lookup, val);
	_promises.push_back(p);
}

void ResourceCtl::_resolvePromises() {
	for (auto p : _promises) {
		if (p._toResolve == -1) {
			auto val = p._lookup.find(p._key);
			if (val != p._lookup.end()) {
				p._toResolve = val->second;
				cout << "resolved " << p._key << " ID: " << val->second << endl;
			}
		}
	}
}

void ResourceCtl::loadResources() {
	_lastAct = 0;
	_loadActionTemplates();
	_loadDefaultActions();
	_loadObjectRecords();
	_loadSprites();
	_loadItemRecords();

	_resolvePromises();
}

void ResourceCtl::_loadObjectRecords() {
	auto allObjects = _conf->getCollection(C_CONFIG_OBJECT, "objects");

	int objID = 0;
	for (auto objInfo : allObjects) {
		auto record = std::dynamic_pointer_cast<JsonObject>(objInfo.second);
		if (record) {
			bool status = true;
			Point mapSize = extractValue<Point>(record->getValue("mapSize"), &status);
			Point sprSize = extractValue<Point>(record->getValue("sprSize"), &status);
			Point offset = extractValue<Point>(record->getValue("offset"), &status);
			int spriteID = extractValue<int>(record->getValue("spriteID"), &status);
			int frames = extractValue<int>(record->getValue("frames"), &status);
			bool lift = extractValue<bool>(record->getValue("lift"), &status);

			if (status) {
				auto info = make_shared<ObjectInfo>(objID, mapSize, sprSize, offset, spriteID, frames, lift);

				auto actPtr = std::dynamic_pointer_cast<JsonTValue<std::vector<shared_ptr<JsonValue>>>>(record->getValue("activeAreas"));
				if (actPtr) {
					auto actAreas = actPtr->getValue();
					for (auto actArea : actAreas) {
						_loadActiveAreas(info, std::dynamic_pointer_cast<JsonObject>(actArea));
					}
				}

				_info.emplace(objID, info);
				_objectLookup.emplace(objInfo.first, objID);
				objID++;
			}
		}
	}
}


void ResourceCtl::_loadActiveAreas(shared_ptr<ObjectInfo> info, shared_ptr<JsonObject> actArea) {
	if (actArea) {
		auto arrPtr = std::dynamic_pointer_cast<JsonTValue<std::vector<shared_ptr<JsonValue>>>>(actArea->getValue("actions"));
		if (arrPtr) {
			Point pos = extractValue<Point>(actArea->getValue("pos"));
			Point size = extractValue<Point>(actArea->getValue("size"));

			auto act_map = make_shared<ObjectActionArea>();
			auto actionArr = arrPtr->getValue();
			for (auto action : actionArr) {
				int actID = _loadAction(std::dynamic_pointer_cast<JsonObject>(action));
				if (_actionInfo.find(actID) != _actionInfo.end()) {
					act_map->_acts.push_back(actID);
				}
			}
			act_map->_area = Rect(pos, size);
			info->addActionArea(act_map);
		}
	}
}

void ResourceCtl::_loadItemRecords() {
	auto allItems = _conf->getCollection(C_CONFIG_ITEM, "items");

	Point sprSize(48, 48);
	Point offset(13, 12);
	int itemID = 0;
	int setID = getSpriteID("itemset_1x1");

	for (auto itemInfo : allItems) {
		auto record = std::dynamic_pointer_cast<JsonObject>(itemInfo.second);
		if (record) {
			bool status = true;
			std::string name = extractValue<std::string>(record->getValue("name"), &status);
			Point size = extractValue<Point>(record->getValue("size"), &status);
			int frame = extractValue<int>(record->getValue("frame"), &status);
			bool consumable = extractValue<bool>(record->getValue("consumable"), &status);

			if (status) {
				auto info = make_shared<ItemInfo>(itemID, name, size, sprSize, offset, setID, frame);

				_itemInfo.emplace(itemID, info);
				_itemLookup.emplace(itemInfo.first, itemID);
				itemID++;
			}
		}
	}
}


// DO NOT LOAD BITMAPS (SPRITES) BEFORE CREATING DISPLAY
void ResourceCtl::_loadSprites() {
	// clear it in case we are reloading (new display is created)
	_sprites.clear();
	_spriteLookup.clear();

	auto spriteRecords = _conf->getCollection(C_CONFIG_RESOURCE, "sprites");
	int sprID = 0;

	for (auto sprRecord : spriteRecords) {
		auto record = std::dynamic_pointer_cast<JsonObject>(sprRecord.second);
		if (record) {
			bool status = true;
			std::string filename = extractValue<std::string>(record->getValue("filename"), &status);
			std::string type = extractValue<std::string>(record->getValue("type"), &status);
			std::string object = extractValue<std::string>(record->getValue("object"));
			shared_ptr<ObjectInfo> objInfo = nullptr;
			
			if (status) {
				if (!object.empty()) {
					objInfo = getObjectInfo(object.c_str());
				}

				if (type.compare("Sprite") == 0) {
					_sprites.emplace(sprID, make_shared<Sprite>(sprID, filename.c_str()));
					_spriteLookup.emplace(sprRecord.first, sprID);
					if (objInfo) {
						objInfo->_spriteID = sprID;
					}
					sprID++;
				} else if (type.compare("SpriteSheet") == 0) {
					Point size = extractValue<Point>(record->getValue("size"), &status);
					int lastID = extractValue<int>(record->getValue("lastID"), &status);
					int rowSize = extractValue<int>(record->getValue("rowSize"), &status);

					if (status) {
						_sprites.emplace(sprID, make_shared<SpriteSheet>(sprID, filename.c_str(), lastID, rowSize, size));
						_spriteLookup.emplace(sprRecord.first, sprID);
						if (objInfo) {
							objInfo->_spriteID = sprID;
						}
						sprID++;
					}
				} else {
					cout  << "ERR: can't recognize sprite type '" << type << "'" << endl;
				}
			}
		}
	}
}


void ResourceCtl::_loadActionTemplates() {
	auto allActions = _conf->getCollection(C_CONFIG_ACTION, "templates");

	for (auto actT : allActions) {
		auto record = std::dynamic_pointer_cast<JsonObject>(actT.second);
		if (record) {
			bool status = true;
			std::string logicName = extractValue<std::string>(record->getValue("logicType"), &status);
			int ticks = extractValue<int>(record->getValue("ticks"), &status);
			int steps = extractValue<int>(record->getValue("steps"), &status);
			auto argPtr = std::dynamic_pointer_cast<JsonTValue<std::vector<shared_ptr<JsonValue>>>>(record->getValue("arguments"));
			auto logicType = matchActionAbstract(logicName);

			if (status && logicType != ACTION_ABSTRACT_LAST) {
				std::vector<std::string> paramList;

				if (argPtr) {
					auto argList = argPtr->getValue();
					for (auto arg : argList) {
						std::string argument = extractValue<std::string>(arg, &status);
						if (status) {
							paramList.push_back(argument);
						}
					}
				}

				auto actTRecord = make_shared<ActionTemplate>(logicType, actT.first, ticks, steps, paramList);
				_actionTemplates.emplace(actT.first, actTRecord);
			}
		}
	}
}


void ResourceCtl::_loadDefaultActions() {
	auto allActions = _conf->getCollection(C_CONFIG_ACTION, "basicActions");

	for (auto actT : allActions) {
		_loadAction(std::dynamic_pointer_cast<JsonObject>(actT.second));
	}
}


int ResourceCtl::_loadAction(shared_ptr<JsonObject> actRecord) {
	if (actRecord) {
		bool status = true;
		std::string type = extractValue<std::string>(actRecord->getValue("type"), &status);
		std::string cmd = extractValue<std::string>(actRecord->getValue("command"), &status);

		if (status) {
			if (_actionLookup.find(cmd) != _actionLookup.end()) {
				cout << endl;
			} else {
				auto it = _actionTemplates.find(type);
				if (it != _actionTemplates.end()) {
					auto actTemp = it->second;
					int objID = -1;
					int itemID = -1;
					int quantity = -1;

					// primary lookup and validation
					for (auto arg = actTemp->params().begin(); status && arg != actTemp->params().end(); arg++) {

						if ((*arg).compare("paramObj") == 0) {
							std::string objName = extractValue<std::string>(actRecord->getValue((*arg)), &status);
							if (status && !_confRecordExists(C_CONFIG_OBJECT, "objects", objName)) {
								cout << "Object " << objName << " doesn't exist" << endl;
								status = false;
							} else {
								objID = getActionID(objName.c_str());
							}
						} else if ((*arg).compare("paramItem") == 0) {
							std::string itemName = extractValue<std::string>(actRecord->getValue((*arg)), &status);
							if (status && !_confRecordExists(C_CONFIG_ITEM, "items", itemName)) {
								cout << "Item " << itemName << " doesn't exist" << endl;
								status = false;
							}
						} else if ((*arg).compare("quantity") == 0) {
							quantity = extractValue<int>(actRecord->getValue((*arg)), &status);
						} else {
							cout << "ERR: Action argument " << (*arg) << " is not recognized" << endl;
							status = false;
						}
					}

					if (status) {
						auto actInfo = make_shared<ActionInfo>(actTemp->type(), actTemp->name(), actTemp->cycles(),
							actTemp->steps(), itemID, objID, quantity);

						for (auto arg = actTemp->params().begin(); arg != actTemp->params().end(); arg++) {
							std::string key = extractValue<std::string>(actRecord->getValue((*arg)), &status);
							if ((*arg).compare("paramObj") == 0 && objID == -1) {
								_makePromise(key, _objectLookup, actInfo->_objParam);
							} else if ((*arg).compare("paramItem") == 0 && itemID == -1) {
								_makePromise(key, _itemLookup, actInfo->_itemParam);
							}
						}
						_actionInfo.emplace(_lastAct, actInfo);
						_actionLookup.emplace(cmd, _lastAct);
						return _lastAct++;
					}
				} else {
					cout << "ERR: Template " << type << " doesn't exist" << endl;
				}
			}
		}
	}
	return -1;
}


int ResourceCtl::getObjectID(const char* name) const {
	auto it = _objectLookup.find(name);
	if (it != _objectLookup.end()) {
		return it->second;
	}
	return -1;
}


shared_ptr<ObjectInfo> ResourceCtl::getObjectInfo(const char* name) const {
	return getObjectInfo(getObjectID(name));
}

shared_ptr<ObjectInfo> ResourceCtl::getObjectInfo(int type) const {
	auto ptr = _info.find(type);
	if (ptr != _info.end()) {
		return ptr->second;
	}
	return shared_ptr<ObjectInfo>();
}


int ResourceCtl::getItemCount() const {
	return _itemInfo.size();
}


int ResourceCtl::getItemID(const char* name) const {
	auto it = _itemLookup.find(name);
	if (it != _itemLookup.end()) {
		return it->second;
	}
	return -1;
}


shared_ptr<ItemInfo> ResourceCtl::getItemInfo(const char* name) const {
	return getItemInfo(getItemID(name));
}


shared_ptr<ItemInfo> ResourceCtl::getItemInfo(int type) const {
	auto ptr = _itemInfo.find(type);
	if (ptr != _itemInfo.end()) {
		return ptr->second;
	}
	return shared_ptr<ItemInfo>();
}


int ResourceCtl::getSpriteID(const char* name) const {
	auto it = _spriteLookup.find(name);
	if (it != _spriteLookup.end()) {
		return it->second;
	}
	return -1;
}


shared_ptr<Sprite> ResourceCtl::getSprite(int id) const {
	auto ptr = _sprites.find(id);
	if (ptr != _sprites.end()) {
		return ptr->second;
	}
	return nullptr;
}


shared_ptr<Sprite> ResourceCtl::getSprite(const char* name) const {
	return getSprite(getSpriteID(name));
}


shared_ptr<SpriteSheet> ResourceCtl::getSpriteSheet(int id) const {
	auto ptr = _sprites.find(id);
	if (ptr != _sprites.end()) {
		return std::dynamic_pointer_cast<SpriteSheet>(ptr->second);
	}
	return nullptr;
}


shared_ptr<SpriteSheet> ResourceCtl::getSpriteSheet(const char* name) const {
	return getSpriteSheet(getSpriteID(name));
}


shared_ptr<SpriteText> ResourceCtl::getFont(int size) {
	if (size > 0 && size < 73) {
		auto ptr = _arialFonts.find(size);
		if (ptr != _arialFonts.end()) {
			return ptr->second;
		} else {
			// add the requested font if not found (basically a cache)

			auto newFont = make_shared<SpriteText>("res/arialbd.ttf", size);
			_arialFonts.emplace(size, newFont);
			return newFont;
		}
	}
	return nullptr;
}


shared_ptr<ActionInfo> ResourceCtl::getActionInfo(const char* name) const {
	return getActionInfo(getActionID(name));
}


shared_ptr<ActionInfo> ResourceCtl::getActionInfo(int id) const {
	auto ptr = _actionInfo.find(id);
	if (ptr != _actionInfo.end()) {
		return ptr->second;
	}
	return nullptr;
}

int ResourceCtl::getActionID(const char* name) const {
	auto it = _actionLookup.find(name);
	if (it != _actionLookup.end()) {
		return it->second;
	}
	return -1;
}


ActionAbstractType ResourceCtl::matchActionAbstract(std::string& name) const {
	if (name.compare("ACTION_ABS_MOVE") == 0) {
		return ACTION_ABS_MOVE;
	} else if (name.compare("ACTION_ABS_GATHER") == 0) {
		return ACTION_ABS_GATHER;
	} else if (name.compare("ACTION_ABS_PICK") == 0) {
		return ACTION_ABS_PICK;
	} else if (name.compare("ACTION_ABS_CARRY") == 0) {
		return ACTION_ABS_CARRY;
	} else if (name.compare("ACTION_ABS_DROP") == 0) {
		return ACTION_ABS_DROP;
	} else if (name.compare("ACTION_ABS_CUT") == 0) {
		return ACTION_ABS_CUT;
	} else if (name.compare("ACTION_ABS_CRAFT") == 0) {
		return ACTION_ABS_CRAFT;
	}
	return ACTION_ABSTRACT_LAST;
}


std::string ResourceCtl::getStatName(StatScoreID id) const {
	switch (id) {
	case C_STAT_STR:
		return "STR";
		break;
	case C_STAT_CON:
		return "CON";
		break;
	case C_STAT_DEX:
		return "DEX";
		break;
	case C_STAT_PER:
		return "PER";
		break;
	case C_STAT_INT:
		return "INT";
		break;
	case C_STAT_WILL:
		return "WILL";
		break;
	case C_STAT_LUCK:
		return "LUCK";
		break;
	case C_STAT_BEA:
		return "BEA";
		break;
	case C_STAT_CHA:
		return "CHA";
		break;
	default:
		break;
	}
	return "ERR";
}

std::string ResourceCtl::getSkillName(SkillScoreID id) const {
	switch (id) {
	case C_SKILL_MELEE:
		return "Melee";
		break;
	case C_SKILL_EXPLORE:
		return "Exploring";
		break;
	case C_SKILL_SURVIVAL:
		return "Survival";
		break;
	case C_SKILL_FARM:
		return "Farming";
		break;
	case C_SKILL_SMITH:
		return "Smithing";
		break;
	case C_SKILL_TAILOR:
		return "Tailoring";
		break;
	default:
		break;
	}
	return "ERR";
}

std::string ResourceCtl::getItemName(int id) const {
	for (auto it : _itemLookup) {
		if (it.second == id) {
			return it.first;
		}
	}
	return "ERR";
}

