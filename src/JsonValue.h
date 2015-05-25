#pragma once
#include <map>
#include <string>
#include <vector>

enum JsonValueType {
	JSON_NULL = 0,
	JSON_BOOLEAN,
	JSON_DOUBLE,
	JSON_STRING,
	JSON_ARRAY,
	JSON_OBJECT,
	JSON_VALUE_INVALID
};

class JsonValue {
	JsonValueType _type;
public:
	JsonValue();
	JsonValue(JsonValueType t);
	JsonValue(const JsonValue& rhs);
	JsonValue& operator= (const JsonValue& rhs);
	virtual ~JsonValue();
	JsonValueType getType() const;
	void setType(JsonValueType t);
};


template<typename T>
class JsonTValue : public JsonValue {
	T _value;
public:
	JsonTValue();
	JsonTValue(const T& val);
	JsonTValue(const JsonTValue& rhs);
	JsonTValue& operator= (const JsonTValue& rhs);
	~JsonTValue();
	void setValue(const T& val);
	T getValue() const;
};


class JsonObject : public JsonValue {
	std::map<std::string, JsonValue> _values;
public:
	JsonObject();
	JsonObject(const JsonObject& rhs);
	JsonObject& operator= (const JsonObject& rhs);
	~JsonObject();
	bool add(const std::string& name, const JsonValue& value);
	void setValue(const std::string& name, const JsonValue& value);
	JsonValue getValue(const std::string& name) const;
	JsonValue getValue(const char* name) const;
};