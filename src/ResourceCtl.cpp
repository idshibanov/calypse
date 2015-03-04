#include "ResourceCtl.h"

ResourceCtl::ResourceCtl() {
	int id = 0;
	_info.emplace(id++, make_shared<ObjectInfo>(id, 44, 69, 10, -57, 3));	// actor
	_info.emplace(id++, make_shared<ObjectInfo>(id, 64, 92, 0, -60, 2));	// reet
	_info.emplace(id++, make_shared<ObjectInfo>(id, 64, 92, 0, -60, 4));	// house
}

ResourceCtl::~ResourceCtl() {

}

// DO NOT CREATE SPRITES BEFORE CREATING DISPLAY
void ResourceCtl::loadSprites() {
	int id = 0;
	_sprites.emplace(id++, make_shared<Sprite>(id, "res/cursor.png"));
	_sprites.emplace(id++, make_shared<Sprite>(id, "res/grass.png"));
	_sprites.emplace(id++, make_shared<Sprite>(id, "res/reet.png"));

	// 44x69, 11-0-11, 6 dirs, 8 frames, T-TR, R , RD, D-LD, L, TL
	auto walk_size = getObjectInfo(0).lock()->_size;
	_sprites.emplace(id++, make_shared<SpriteSheet>(id, "res/f2_walk.png", 48, 8, Point(walk_size._x, walk_size._y)));

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