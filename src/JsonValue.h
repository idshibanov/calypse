#pragma once
#include <map>
#include <string>
#include <vector>

#include "Main.h"

enum JsonValueType {
	JSON_NULL = 0,
	JSON_BOOLEAN,
	JSON_INTEGER,
	JSON_DOUBLE,
	JSON_STRING,
	JSON_ARRAY,
	JSON_OBJECT,
	JSON_VALUE_INVALID
};

class JsonValue {
protected:
	JsonValueType _type;
public:
	JsonValue();
	JsonValue(JsonValueType t);
	JsonValue(const JsonValue& rhs);
	JsonValue& operator= (const JsonValue& rhs);
	virtual ~JsonValue();
	JsonValueType getType() const;
	virtual void print(std::ostream& out) const;
	friend std::ostream& operator<<(std::ostream& str, const JsonValue& v);
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
	void determineType();
	void setValue(const T& val);
	T getValue() const;
	virtual void print(std::ostream& out) const;
	friend std::ostream& operator<<(std::ostream& out, const JsonTValue<T>& v);
};


class JsonObject : public JsonValue {
	std::map<std::string, shared_ptr<JsonValue>> _values;
public:
	JsonObject();
	JsonObject(const JsonObject& rhs);
	JsonObject& operator= (const JsonObject& rhs);
	~JsonObject();
	bool add(const std::string& name, shared_ptr<JsonValue> value);
	void setValue(const std::string& name, shared_ptr<JsonValue> value);
	shared_ptr<JsonValue> getValue(const std::string& name) const;
	shared_ptr<JsonValue> getValue(const char* name) const;
	const std::map<std::string, shared_ptr<JsonValue>>& getContents() const;
	virtual void print(std::ostream& out) const;
	friend std::ostream& operator<<(std::ostream& str, const JsonObject& v);
};