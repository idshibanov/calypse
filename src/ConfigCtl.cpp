#include <fstream>
#include <sstream>

#include "ConfigCtl.h"

ConfigCtl::ConfigCtl() {
	loadFile("config/app.json", _app);
	loadFile("config/res.json", _res);
	loadFile("config/ui.json", _ui);
	loadFile("config/action.json", _action);
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
		return true;
	} else {
		throw std::string("Could not load a config file: ").append(filename);
	}
	return false;
}

shared_ptr<JsonValue> ConfigCtl::getSetting(ConfigCategory cat, const std::string& param) {
	switch (cat) {
	case C_CONFIG_APP:
		return _app.getValue(param);
		break;
	case C_CONFIG_RESOURCE:
		return _res.getValue(param);
		break;
	case C_CONFIG_UI:
		return _ui.getValue(param);
		break;
	case C_CONFIG_ACTION:
		return _action.getValue(param);
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


const std::map<std::string, shared_ptr<JsonValue>>& ConfigCtl::getCollection(ConfigCategory cat, const std::string& param) {
	auto objT = getSetting(cat, param);
	if (objT && objT->getType() == JSON_OBJECT) {
		auto objConf = std::dynamic_pointer_cast<JsonObject>(objT);
		if (objConf) {
			return objConf->getContents();
		}
	}
	return std::map<std::string, shared_ptr<JsonValue>>();
}