#include <fstream>
#include <sstream>

#include "ConfigCtl.h"

ConfigCtl::ConfigCtl() {
	loadFile("config/app.json", _app);
	loadFile("config/res.json", _res);
	loadFile("config/obj.json", _obj);
	loadFile("config/item.json", _item);
}

ConfigCtl::~ConfigCtl() {

}

bool ConfigCtl::loadFile(const char* filename, JsonObject& obj) {
	std::fstream tmpfile;
	std::stringstream buffer;
	tmpfile.open(filename);
	if (tmpfile.is_open()) {
		buffer << tmpfile.rdbuf();
		obj = parseJsonString(buffer.str());
		tmpfile.close();
	} else {
		throw std::string("Could not load a config file: ").append(filename);
	}
}

shared_ptr<JsonValue> ConfigCtl::getSetting(ConfigCategory cat, const std::string& param) {
	switch (cat) {
	case C_CONFIG_APP:
		return _app.getValue(param);
		break;
	case C_CONFIG_RESOURCE:
		return _res.getValue(param);
		break;
	case C_CONFIG_OBJECT:
		return _obj.getValue(param);
		break;
	case C_CONFIG_ITEM:
		return _item.getValue(param);
		break;
	}
	return nullptr;
}