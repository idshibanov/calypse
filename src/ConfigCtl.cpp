#include <fstream>
#include <sstream>

#include "ConfigCtl.h"

ConfigCtl::ConfigCtl() {
	std::stringstream buffer;
	std::fstream appfile;
	appfile.open("app.json");
	buffer << appfile.rdbuf();
	_app = parseJsonString(buffer.str());
	appfile.close();

	std::ofstream myfile;
	myfile.open("myfile.json");
	myfile << _app;
	myfile.close();
}

ConfigCtl::~ConfigCtl() {

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