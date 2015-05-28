#pragma once
#include "JsonValue.h"

JsonObject parseJsonString(const std::string& str);

// helper functions
void skipWhitespace(std::istringstream& is);
void skipWhitespace(std::string& str);
std::vector<std::string> splitString(std::istringstream& is, const char delim);
std::string extractString(std::string& str);

bool parsePair(std::string& pair, JsonObject& parent);
bool parseValue(std::string& str, std::string& key, JsonObject& parent);

std::string parseName(std::string& key);
shared_ptr<JsonValue> parseValue(std::string& str);

shared_ptr<JsonValue> parseNull(std::istringstream& is);
shared_ptr<JsonValue> parseTrue(std::istringstream& is);
shared_ptr<JsonValue> parseFalse(std::istringstream& is);
shared_ptr<JsonValue> parseInteger(std::istringstream& is);
shared_ptr<JsonValue> parseString(std::istringstream& is);
shared_ptr<JsonValue> parseArray(std::istringstream& is);
shared_ptr<JsonValue> parseObject(std::istringstream& is);