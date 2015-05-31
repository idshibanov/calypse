#include <sstream>

#include "Main.h"
#include "JsonParser.h"


JsonObject parseJsonString(const std::string& str) {
	JsonObject top;
	size_t start = str.find('{');

	if (start != std::string::npos) {
		auto tokens = splitString(str, start+1);
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

size_t skipNested(const std::string lookup, size_t curr, const char& open, const char& close) {
	int depth = 1;
	while (depth > 0 && curr < lookup.length()) {
		curr++;
		if (lookup[curr] == open) {
			// check if we're working with the same element, such as "
			if (open == close) {
				// it cannot be nested and it must be escaped
				// if it's not the case then we close it of
				if (!curr || lookup[curr - 1] != '\\') {
					depth--;
				}
			} else {
				depth++;
			}
		} else if (lookup[curr] == close) {
			depth--;
		}
	}
	return curr;
}

std::vector<std::string> splitString(const std::string& str, size_t start) {
	std::vector<std::string> retval;

	size_t end = str.find(',', start);
	while (end != std::string::npos) {
		std::string token = str.substr(start, end - start);

		// Inner array/object search - we have to take them as a whole
		// and put in the pair - parsePair() will do its work
		size_t objS = token.find('{');
		size_t arrS = token.find('[');
		size_t n = std::count(token.begin(), token.end(), '"');
		size_t strS = token.find_last_of('"');

		if (objS != arrS || n % 2) {
			size_t curr = start;

			// Skimming through the original string, looking for nested arrays/objects
			if (objS < arrS && objS < strS) {
				curr = skipNested(str, curr + objS, '{', '}');
			} else if (arrS < objS && arrS < strS) {
				curr = skipNested(str, curr + arrS, '[', ']');
			} else {
				curr = skipNested(str, curr + strS, '"', '"');
			}

			// update the end position with proper value & grab the whole pair
			end = str.find(',', curr);
			token = str.substr(start, end - start);

			// start may overflow (end+1), so we take care of that
			if (end == std::string::npos) {
				end = str.length();
			}
		}

		if (!token.empty()) {
			retval.push_back(token);
		}

		start = end + 1;
		end = str.find(',', start);
	}

	// check if there is last pair left
	if (start < str.length()) {
		retval.push_back(str.substr(start));
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


bool parsePair(std::string& pair, JsonObject& parent, size_t start) {
	size_t split = pair.find(':', start);
	if (split != std::string::npos) {
		auto name = parseName(pair.substr(start, split));
		if (!name.empty()) {
			return parseValue(pair.substr(split+1), name, parent);
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
		retval = parseArray(str);
		break;
	case '{':
		retval = parseObject(str);
		break;
	default:
		retval = parseNumber(is);
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
		return parseArray(str);
	case '{':
		return parseObject(str);
	}
	return parseNumber(is);
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

shared_ptr<JsonValue> parseNumber(std::istringstream& is) {
	std::string strValue;

	std::getline(is, strValue);
	if (!strValue.empty()) {
		bool decimal = strValue.find(".") != std::string::npos;
		// Personally I'm not into C++ exceptions, but this is a fine solution, so bear with this inconsistency
		try {
			if (decimal) {
				double value = std::stod(strValue);
				return make_shared<JsonTValue<double>>(value);
			} else {
				int value = std::stoi(strValue);
				return make_shared<JsonTValue<int>>(value);
			}
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

shared_ptr<JsonValue> parseArray(const std::string& src) {
	size_t start = src.find('[');

	if (start != std::string::npos) {
		auto tokens = splitString(src, start + 1);
		std::vector<shared_ptr<JsonValue>> holder;
			
		for (auto value : tokens) {
			holder.push_back(parseValue(value));
		}
		return make_shared<JsonTValue<std::vector<shared_ptr<JsonValue>>>>(holder);
	}
	return nullptr;
}

shared_ptr<JsonValue> parseObject(const std::string& src) {
	shared_ptr<JsonObject> job = nullptr;
	size_t start = src.find('{');

	if (start != std::string::npos) {
		job = make_shared<JsonObject>();

		auto tokens = splitString(src, start+1);
		for (auto token : tokens) {
			if (!parsePair(token, (*job))) {
				cout << "Could not parse the following name/value pair:" << endl;
				cout << token << endl;
			}
		}
	}
	return job;
}


template<typename T>
T extractValue(shared_ptr<JsonValue> v) {
	shared_ptr<JsonTValue<T>> ptr = std::dynamic_pointer_cast<JsonTValue<T>>(v);
	if (ptr) {
		return ptr->getValue();
	}
	return T();
}

template <>
Point extractValue<Point>(shared_ptr<JsonValue> v) {
	auto val = std::dynamic_pointer_cast<JsonTValue<std::string>>(v);
	if (val) {
		auto str = val->getValue();
		size_t pos = str.find(',');
		if (pos != std::string::npos) {
			try {
				int first = std::stoi(str.substr(0, pos));
				int second = std::stoi(str.substr(pos + 1));
				return Point(first, second);
			}
			catch (...) {
				// stoi failed, either overflow or illegal characters
			}
		}
	}
	return Point();
}

template bool extractValue<bool>(shared_ptr<JsonValue> v);
template int extractValue<int>(shared_ptr<JsonValue> v);
template double extractValue<double>(shared_ptr<JsonValue> v);
template std::string extractValue<std::string>(shared_ptr<JsonValue> v);
template Point extractValue<Point>(shared_ptr<JsonValue> v);