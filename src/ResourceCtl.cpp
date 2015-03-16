#include "ResourceCtl.h"

ResourceCtl::ResourceCtl() {
	int id = 0;
	_info.emplace(id++, make_shared<ObjectInfo>(id, 44, 69, 10, -57, 3, false));	// actor
	_info.emplace(id++, make_shared<ObjectInfo>(id, 64, 92, 0, -60, 2, false)); 	// reet
	_info.emplace(id++, make_shared<ObjectInfo>(id, 160, 153, -96, -73, 4, false));	// hut
	_info.emplace(id++, make_shared<ObjectInfo>(id, 96, 51, -18, -6, 5, true));	// hide
}

ResourceCtl::~ResourceCtl() {

}

// DO NOT CREATE SPRITES BEFORE CREATING DISPLAY
void ResourceCtl::loadSprites() {
	int id = 0;
	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/cursor_sheet.png", 2, 2, Point(32,32)));
	_sprites.emplace(id++, make_shared<Sprite>(id, "res/grass.png"));

	auto reet_size = getObjectInfo(1).lock()->_size;
	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/reet_sheet.png", 2, 2, reet_size));

	// 44x69, 11-0-11, 6 dirs, 8 frames, T-TR, R , RD, D-LD, L, TL
	auto walk_size = getObjectInfo(0).lock()->_size;
	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/f2_walk.png", 48, 8, walk_size));

	auto hut_size = getObjectInfo(2).lock()->_size;
	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/hut.png", 1, 1, hut_size));

	auto hide_size = getObjectInfo(3).lock()->_size;
	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/bear_hide.png", 1, 1, hide_size));
}

weak_ptr<ObjectInfo> ResourceCtl::getObjectInfo(int type) {
	auto ptr = _info.find(type);
	if (ptr != _info.end()) {
		return ptr->second;
	}
	return weak_ptr<ObjectInfo>();
}

shared_ptr<Sprite> ResourceCtl::getSprite(int id) {
	auto ptr = _sprites.find(id);
	if (ptr != _sprites.end()) {
		return ptr->second;
	}
	return nullptr;
}