#pragma once
#include "JsonValue.h"

JsonObject parseJsonString(const std::string& str);

// helper functions
void skipWhitespace(std::istringstream& is);
std::string getName(std::istringstream& is);

bool parseValue(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseNull(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseTrue(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseFalse(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseInteger(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseString(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseArray(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseObject(std::istringstream& is, std::string& key, JsonObject& parent);