#include "ResourceCtl.h"

ResourceCtl::ResourceCtl() {
	int id = 0;
	_info.emplace(id++, make_shared<ObjectInfo>(id, Point(2, 2), Point(44, 69), Point(10, -57), 3, 48, false));	// actor

	auto reet_info = make_shared<ObjectInfo>(id, Point(10, 10), Point(64, 92), Point(0, -60), 2, 2, false);
	auto reet_map = make_shared<ObjectActionArea>();
	reet_map->_area = Rect(Point(), Point(64, 92));
	reet_map->_acts.push_back(ACTION_CUT);
	reet_map->_acts.push_back(ACTION_PICK_BRANCH);
	reet_map->_acts.push_back(ACTION_CUT);
	reet_map->_acts.push_back(ACTION_PICK_BRANCH);
	reet_info->addActionArea(reet_map);
	_info.emplace(id++, reet_info); 	// reet

	_info.emplace(id++, make_shared<ObjectInfo>(id, Point(20, 30), Point(160, 153), Point(-96, -73), 4, 1, false));	// hut
	_info.emplace(id++, make_shared<ObjectInfo>(id, Point(1, 1), Point(96, 51), Point(-18, -6), 5, 1, true));	// hide
	_info.emplace(id++, make_shared<ObjectInfo>(id, Point(8, 8), Point(48, 58), Point(8, -26), 6, 1, true));	// fire

	_itemInfo.emplace(C_ITEM_BRANCH, make_shared<ItemInfo>(C_ITEM_BRANCH, Point(9,8), Point(32,32), 7));
}

ResourceCtl::~ResourceCtl() {

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

	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/items.png", 1, 1, Point(32,32)));

	_sprites.emplace(id++, make_shared<Sprite>(id, "res/buttonG32.png"));
	_sprites.emplace(id++, make_shared<Sprite>(id, "res/buttonR32.png"));

	// fonts map, specified as a map
	_arialFonts.emplace(12, make_shared<SpriteText>("res/arialbd.ttf", 12));
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
	case C_ITEM_BRANCH:
		return "Branches";
		break;
	case C_ITEM_CONE:
		return "Cones";
		break;
	case C_ITEM_APPLE:
		return "Apples";
		break;
	}
	return "ERR";
}
