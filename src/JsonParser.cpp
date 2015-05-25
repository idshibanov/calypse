#include <sstream>

#include "Main.h"
#include "JsonParser.h"


JsonObject parseJsonString(const std::string& str) {
	JsonObject top;


	std::istringstream is(str);
	skipWhitespace(is);
	size_t pos = str.find("{");

	if (pos == std::string::npos) {
		//parseObject(is, top);
	}
	return top;
}


void skipWhitespace(std::istringstream& is) {
	std::streamsize cnt = 0;
	int sym = is.peek();
	while (sym == ' ' || sym == '\n' || sym == '\r' || sym == '\t') {
		cnt++;
		sym = is.peek();
	}
	if (cnt) {
		is.ignore(cnt);
	}
}

std::string getName(std::istringstream& is) {
	return std::string();
}

bool parseValue(std::istringstream& is, std::string& key, JsonObject& parent) {
	switch (is.peek()) {
	case 'n':
		return parseNull(is, key, parent);
	case 't':
		return parseTrue(is, key, parent);
	case 'f':
		return parseFalse(is, key, parent);
	case '"':
		return parseString(is, key, parent);
	case '[':
		return parseArray(is, key, parent);
	case '{':
		return parseObject(is, key, parent);
	}
	return parseInteger(is, key, parent);
}

bool parseNull(std::istringstream& is, std::string& key, JsonObject& parent) {
	char val[4];
	is.read(val, 4);
	std::string value(val);

	if (value.find("null") >= 0) {
		parent.add(key, JsonValue());
		return true;
	}
	return false;
}

bool parseTrue(std::istringstream& is, std::string& key, JsonObject& parent) {
	char val[4];
	is.read(val, 4);
	std::string value(val);

	if (value.find("true") >= 0) {
		parent.add(key, JsonTValue<bool>(true));
		return true;
	}
	return false;
}

bool parseFalse(std::istringstream& is, std::string& key, JsonObject& parent) {
	char val[5];
	is.read(val, 5);
	std::string value(val);

	if (value.find("false") >= 0) {
		parent.add(key, JsonTValue<bool>(false));
		return true;
	}
	return false;
}

bool parseInteger(std::istringstream& is, std::string& key, JsonObject& parent) {
	std::string value;
	std::getline(is, value, ',');
	cout << value;
	/*
	std::getline(ss, s, '=');                     // gives me "a"
	std::streampos  save = ss.tellg();
	std::getline(ss, s);                          // gives me "b+c"
	ss.seekg(save);                               // Roll back
	std::getline(ss, s, '+');                     // gives me "b"
	*/
	return false;
}

bool parseString(std::istringstream& is, std::string& key, JsonObject& parent) {
	return false;
}

bool parseArray(std::istringstream& is, std::string& key, JsonObject& parent) {
	return false;
}

bool parseObject(std::istringstream& is, std::string& key, JsonObject& parent) {
	return false;
}
