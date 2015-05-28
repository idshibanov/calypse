#include <sstream>

#include "Main.h"
#include "JsonParser.h"


JsonObject parseJsonString(const std::string& str) {
	JsonObject top;

	std::istringstream is(str);
	skipWhitespace(is);
	if (is.peek() == '{') {
		is.ignore();
		auto tokens = splitString(is, ',');
		for (auto token : tokens) {
			if (!parsePair(token, top)) {
				cout << "Could not parse the following name/value pair:" << endl;
				cout << token << endl;
			}
		}
	}
	return top;
}


void skipWhitespace(std::istringstream& is) {
	int sym = is.peek();
	while (sym == ' ' || sym == '\n' || sym == '\r' || sym == '\t') {
		is.ignore();
		sym = is.peek();
	}
}

std::vector<std::string> splitString(std::istringstream& is, const char delim) {
	std::vector<std::string> retval;
	std::string token;

	while (is.good()) {
		std::getline(is, token, delim);
		if (!token.empty()) {
			retval.push_back(token);
		}
	}

	return retval;
}

std::string extractString(std::istringstream& is) {
	std::string retval;

	skipWhitespace(is);
	if (is.peek() == '"') {
		is.ignore();
		std::getline(is, retval, '"');
	}
	return retval;
}


bool parsePair(std::string& pair, JsonObject& parent) {
	std::istringstream is(pair);
	skipWhitespace(is);
	auto tokens = splitString(is, ':');
	
	if (tokens.size() == 2) {
		auto name = parseName(tokens[0]);
		if (!name.empty()) {
			return parseValue(tokens[1], name, parent);
		}
	}
	return false;
}

std::string parseName(std::string& key) {
	std::istringstream is(key);	
	return extractString(is);
}

bool parseValue(std::string& str, std::string& key, JsonObject& parent) {
	std::istringstream is(str);
	skipWhitespace(is);

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
		parent.add(key, make_shared<JsonValue>());
		return true;
	}
	return false;
}

bool parseTrue(std::istringstream& is, std::string& key, JsonObject& parent) {
	char val[4];
	is.read(val, 4);
	std::string value(val);

	if (value.find("true") >= 0) {
		parent.add(key, make_shared<JsonTValue<bool>>(true));
		return true;
	}
	return false;
}

bool parseFalse(std::istringstream& is, std::string& key, JsonObject& parent) {
	char val[5];
	is.read(val, 5);
	std::string value(val);

	if (value.find("false") >= 0) {
		parent.add(key, make_shared<JsonTValue<bool>>(false));
		return true;
	}
	return false;
}

bool parseInteger(std::istringstream& is, std::string& key, JsonObject& parent) {
	std::string strValue;

	std::getline(is, strValue);
	if (!strValue.empty()) {
		// Personally I'm not into C++ exceptions, but this is a fine solution, so bear with this inconsistency
		try {
			int value = std::stoi(strValue);
			parent.add(key, make_shared<JsonTValue<int>>(value));
			return true;
		} catch (...) {
			// stoi failed, either overflow or illegal characters
		}
	}

	return false;
}

bool parseString(std::istringstream& is, std::string& key, JsonObject& parent) {
	std::string value = extractString(is);
	if (!value.empty()) {
		parent.add(key, make_shared<JsonTValue<std::string>>(value));
		return true;
	}
	return false;
}

bool parseArray(std::istringstream& is, std::string& key, JsonObject& parent) {
	std::string arrStr;
	skipWhitespace(is);
	if (is.peek() == '[') {
		is.ignore();
		std::getline(is, arrStr, ']');

		if (!arrStr.empty()) {
			auto values = splitString(std::istringstream(arrStr), ',');
			std::vector<shared_ptr<JsonValue>> holder;
			
			for (auto value : values) {
				//parseValue(value, )
			}

			return true;
		}
	}
	return false;
}

/*
std::getline(ss, s, '=');                     // gives me "a"
std::streampos  save = ss.tellg();
std::getline(ss, s);                          // gives me "b+c"
ss.seekg(save);                               // Roll back
std::getline(ss, s, '+');                     // gives me "b"
*/

bool parseObject(std::istringstream& is, std::string& key, JsonObject& parent) {
	return false;
}
