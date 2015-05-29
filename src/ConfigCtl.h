#pragma once
#include "JsonParser.h"

enum ConfigCategory {
	C_CONFIG_APP = 0,
	C_CONFIG_RESOURCE,
	C_CONFIG_OBJECT,
	C_CONFIG_ITEM,
	C_CONFIG_CATEGORY_LAST
};

class ConfigCtl {
	JsonObject _app;
	JsonObject _res;
	JsonObject _obj;
	JsonObject _item;

public:
	ConfigCtl();
	~ConfigCtl();
	shared_ptr<JsonValue> getSetting(ConfigCategory cat, const std::string& param);
};