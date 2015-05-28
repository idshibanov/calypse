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

bool parseValue(std::string& str, std::string& key, JsonObject& parent) {
	shared_ptr<JsonValue> retval = nullptr;
	std::istringstream is(str);

	skipWhitespace(is);
	switch (is.peek()) {
	case 'n':
		retval = parseNull(is);
		break;
	case 't':
		retval = parseTrue(is);
		break;
	case 'f':
		retval = parseFalse(is);
		break;
	case '"':
		retval = parseString(is);
		break;
	case '[':
		retval = parseArray(is);
		break;
	case '{':
		retval = parseObject(is);
		break;
	default:
		retval = parseInteger(is);
	}

	if (retval) {
		parent.add(key, retval);
		return true;
	}
	return false;
}


std::string parseName(std::string& key) {
	std::istringstream is(key);
	return extractString(is);
}

shared_ptr<JsonValue> parseValue(std::string& str) {
	std::istringstream is(str);
	skipWhitespace(is);

	switch (is.peek()) {
	case 'n':
		return parseNull(is);
	case 't':
		return parseTrue(is);
	case 'f':
		return parseFalse(is);
	case '"':
		return parseString(is);
	case '[':
		return parseArray(is);
	case '{':
		return parseObject(is);
	}
	return parseInteger(is);
}


shared_ptr<JsonValue> parseNull(std::istringstream& is) {
	char val[4];
	is.read(val, 4);
	std::string value(val);

	if (value.find("null") >= 0) {
		return make_shared<JsonValue>();
	}
	return nullptr;
}

shared_ptr<JsonValue> parseTrue(std::istringstream& is) {
	char val[4];
	is.read(val, 4);
	std::string value(val);

	if (value.find("true") >= 0) {
		return make_shared<JsonTValue<bool>>(true);
	}
	return nullptr;
}

shared_ptr<JsonValue> parseFalse(std::istringstream& is) {
	char val[5];
	is.read(val, 5);
	std::string value(val);

	if (value.find("false") >= 0) {
		return make_shared<JsonTValue<bool>>(false);
	}
	return nullptr;
}

shared_ptr<JsonValue> parseInteger(std::istringstream& is) {
	std::string strValue;

	std::getline(is, strValue);
	if (!strValue.empty()) {
		// Personally I'm not into C++ exceptions, but this is a fine solution, so bear with this inconsistency
		try {
			int value = std::stoi(strValue);
			return make_shared<JsonTValue<int>>(value);
		} catch (...) {
			// stoi failed, either overflow or illegal characters
		}
	}

	return nullptr;
}

shared_ptr<JsonValue> parseString(std::istringstream& is) {
	std::string value = extractString(is);
	if (!value.empty()) {
		return make_shared<JsonTValue<std::string>>(value);
	}
	return nullptr;
}

shared_ptr<JsonValue> parseArray(std::istringstream& is) {
	std::string arrStr;
	skipWhitespace(is);
	if (is.peek() == '[') {
		is.ignore();
		std::getline(is, arrStr, ']');

		if (!arrStr.empty()) {
			auto values = splitString(std::istringstream(arrStr), ',');
			std::vector<shared_ptr<JsonValue>> holder;
			
			for (auto value : values) {
				holder.push_back(parseValue(value));
			}
			return make_shared<JsonTValue<std::vector<shared_ptr<JsonValue>>>>(holder);
		}
	}
	return nullptr;
}

shared_ptr<JsonValue> parseObject(std::istringstream& is) {
	shared_ptr<JsonObject> job = nullptr;

	skipWhitespace(is);
	if (is.peek() == '{') {
		is.ignore();
		job = make_shared<JsonObject>();

		auto tokens = splitString(is, ',');
		for (auto token : tokens) {
			if (!parsePair(token, (*job))) {
				cout << "Could not parse the following name/value pair:" << endl;
				cout << token << endl;
			}
		}
	}
	return job;
}
