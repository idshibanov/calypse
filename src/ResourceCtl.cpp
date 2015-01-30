#include "ResourceCtl.h"

ResourceCtl::ResourceCtl() {
	_info.emplace(0, ObjectInfo(0, 44, 69, 10, -45));	// actor
	_info.emplace(1, ObjectInfo(1, 64, 92, 0, -60));	// reet
}

ResourceCtl::~ResourceCtl() {

}

unordered_map<short, ObjectInfo>::iterator ResourceCtl::getObjectInfo(int type) {
	return _info.find(type);
}