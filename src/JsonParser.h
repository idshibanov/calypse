#pragma once
#include "JsonValue.h"

JsonObject parseJsonString(const std::string& str);
JsonObject parseJsonStream(const std::string& str);

// helper functions
void skipWhitespace(std::istringstream& is);
std::string extractString(std::istringstream& is);
std::vector<std::string> splitString(const std::string& str, size_t start = 0);

bool parsePair(std::string& pair, JsonObject& parent, size_t start = 0);
bool parseValue(std::string& str, std::string& key, JsonObject& parent);

std::string parseName(std::string& key);
shared_ptr<JsonValue> parseValue(std::string& str);

shared_ptr<JsonValue> parseNull(std::istringstream& is);
shared_ptr<JsonValue> parseTrue(std::istringstream& is);
shared_ptr<JsonValue> parseFalse(std::istringstream& is);
shared_ptr<JsonValue> parseNumber(std::istringstream& is);
shared_ptr<JsonValue> parseString(std::istringstream& is);
shared_ptr<JsonValue> parseArray(const std::string& src);
shared_ptr<JsonValue> parseObject(const std::string& src);