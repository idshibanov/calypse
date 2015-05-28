#pragma once
#include "JsonValue.h"

JsonObject parseJsonString(const std::string& str);

// helper functions
void skipWhitespace(std::istringstream& is);
void skipWhitespace(std::string& str);
std::vector<std::string> splitString(std::istringstream& is, const char delim);
std::string extractString(std::string& str);

bool parsePair(std::string& pair, JsonObject& parent);
std::string parseName(std::string& key);
bool parseValue(std::string& str, std::string& key, JsonObject& parent);

bool parseNull(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseTrue(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseFalse(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseInteger(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseString(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseArray(std::istringstream& is, std::string& key, JsonObject& parent);
bool parseObject(std::istringstream& is, std::string& key, JsonObject& parent);