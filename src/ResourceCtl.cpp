#include "ResourceCtl.h"

ResourceCtl::ResourceCtl(shared_ptr<ConfigCtl> conf) {
	_conf = conf;

	auto objT = _conf->getSetting(C_CONFIG_OBJECT, "objects");
	if (objT && objT->getType() == JSON_OBJECT) {
		auto objConf = std::dynamic_pointer_cast<JsonObject>(objT);
		if (objConf)
			loadObjectRecords(objConf);
	}

	_itemInfo.emplace(C_ITEM_WOOD, make_shared<ItemInfo>(C_ITEM_WOOD, Point(13, 12), Point(48, 48), 7));
	_itemInfo.emplace(C_ITEM_BERRY_RED, make_shared<ItemInfo>(C_ITEM_BERRY_RED, Point(13, 12), Point(48, 48), 7));
	_itemInfo.emplace(C_ITEM_BERRY_BLUE, make_shared<ItemInfo>(C_ITEM_BERRY_BLUE, Point(13, 12), Point(48, 48), 7));
	_itemInfo.emplace(C_ITEM_STONE, make_shared<ItemInfo>(C_ITEM_STONE, Point(13, 12), Point(48, 48), 7));
}

ResourceCtl::~ResourceCtl() {

}

void ResourceCtl::loadObjectRecords(shared_ptr<JsonObject> objConf) {
	auto allObjects = objConf->getContents();

	int objID = 0;
	for (auto objInfo : allObjects) {
		auto record = std::dynamic_pointer_cast<JsonObject>(objInfo.second);
		if (record) {
			Point mapSize = extractValue<Point>(record->getValue("mapSize"));
			Point sprSize = extractValue<Point>(record->getValue("sprSize"));
			Point offset = extractValue<Point>(record->getValue("offset"));
			int spriteID = extractValue<int>(record->getValue("spriteID"));
			int frames = extractValue<int>(record->getValue("frames"));
			bool lift = extractValue<bool>(record->getValue("lift"));

			auto actAreas = std::dynamic_pointer_cast<JsonTValue<std::vector<shared_ptr<JsonValue>>>>(record->getValue("activeAreas"));

			_info.emplace(objID, make_shared<ObjectInfo>(objID, mapSize, sprSize, offset, spriteID, frames, lift));
			_objectLookup.emplace(objInfo.first, objID);
			objID++;
		}
	}
}

// DO NOT CREATE SPRITES BEFORE CREATING DISPLAY
void ResourceCtl::loadSprites() {
	int id = 0;
	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/cursor_sheet.png", 2, 2, Point(32, 32)));
	_sprites.emplace(id++, make_shared<Sprite>(id, "res/grass.png"));

	auto reet_size = getObjectInfo(1)->sprSize();
	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/reet_sheet.png", 2, 2, reet_size));

	// 44x69, 11-0-11, 6 dirs, 8 frames, T-TR, R , RD, D-LD, L, TL
	auto walk_size = getObjectInfo(0)->sprSize();
	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/f2_walk.png", 48, 8, walk_size));

	auto hut_size = getObjectInfo(2)->sprSize();
	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/hut.png", 1, 1, hut_size));

	auto hide_size = getObjectInfo(3)->sprSize();
	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/bear_hide.png", 1, 1, hide_size));

	auto fire_size = getObjectInfo(4)->sprSize();
	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/fire.png", 1, 1, fire_size));

	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/itemset.png", 4, 4, Point(48,48)));

	_sprites.emplace(id++, make_shared<Sprite>(id, "res/buttonG32.png"));
	_sprites.emplace(id++, make_shared<Sprite>(id, "res/buttonR32.png"));

	// fonts map, specified as a map
	_arialFonts.emplace(12, make_shared<SpriteText>("res/arialbd.ttf", 12));
}


int ResourceCtl::getObjectID(const char* name) const {
	auto it = _objectLookup.find(name);
	if (it != _objectLookup.end()) {
		return it->second;
	}
	return -1;
}


shared_ptr<ObjectInfo> ResourceCtl::getObjectInfo(int type) const {
	auto ptr = _info.find(type);
	if (ptr != _info.end()) {
		return ptr->second;
	}
	return shared_ptr<ObjectInfo>();
}


shared_ptr<ItemInfo> ResourceCtl::getItemInfo(ItemType t) const {
	auto ptr = _itemInfo.find(t);
	if (ptr != _itemInfo.end()) {
		return ptr->second;
	}
	return shared_ptr<ItemInfo>();
}


shared_ptr<Sprite> ResourceCtl::getSprite(int id) const {
	auto ptr = _sprites.find(id);
	if (ptr != _sprites.end()) {
		return ptr->second;
	}
	return nullptr;
}


shared_ptr<SpriteSheet> ResourceCtl::getSpriteSheet(int id) const {
	auto ptr = _sprites.find(id);
	if (ptr != _sprites.end()) {
		return std::dynamic_pointer_cast<SpriteSheet>(ptr->second);
	}
	return nullptr;
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


std::string ResourceCtl::getActionName(ActionType id) const {
	switch (id) {
	case ACTION_MOVE:
		return "Move here";
		break;
	case ACTION_CUT:
		return "Cut the tree";
		break;
	case ACTION_DRAG:
		return "Pick up the object";
		break;
	case ACTION_DROP:
		return "Drop the object";
		break;
	case ACTION_CRAFT_TREE:
		return "Plant the tree";
		break;
	case ACTION_PICK_BRANCH:
		return "Pick branch";
		break;
	case ACTION_PICK_RED_BERRY:
		return "Pick raspberry";
		break;
	case ACTION_PICK_BLUE_BERRY:
		return "Pick blueberry";
		break;
	default:
		break;
	}
	return "ERR";
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

std::string ResourceCtl::getItemName(ItemType id) const {
	switch (id) {
	case C_ITEM_WOOD:
		return "Log";
		break;
	case C_ITEM_BERRY_RED:
		return "Red Berry";
		break;
	case C_ITEM_BERRY_BLUE:
		return "Blue Berry";
		break;
	case C_ITEM_STONE:
		return "Stone";
		break;
	default:
		break;
	}
	return "ERR";
}
