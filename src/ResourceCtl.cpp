#include "ResourceCtl.h"

ResourceCtl::ResourceCtl() {
	_info.emplace(0, make_shared<ObjectInfo>(0, 44, 69, 10, -45));	// actor
	_info.emplace(1, make_shared<ObjectInfo>(1, 64, 92, 0, -60));	// reet
}

ResourceCtl::~ResourceCtl() {

}

weak_ptr<ObjectInfo> ResourceCtl::getObjectInfo(short type) {
	auto ptr = _info.find(type);
	if (ptr != _info.end()) {
		return ptr->second;
	}
	return weak_ptr<ObjectInfo>();
}