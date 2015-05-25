#include "JsonValue.h"


JsonValue::JsonValue() {
	_type = JSON_NULL;
}

JsonValue::JsonValue(JsonValueType t) {
	_type = t;
}

JsonValue::JsonValue(const JsonValue& rhs) {
	_type = rhs._type;
}

JsonValue& JsonValue::operator= (const JsonValue& rhs) {
	_type = rhs._type;
	return *this;
}

JsonValue::~JsonValue() {

}

JsonValueType JsonValue::getType() const {
	return _type;
}

void JsonValue::setType(JsonValueType t) {
	_type = t;
}



template<typename T>
JsonTValue<T>::JsonTValue() {

}

template<typename T>
JsonTValue<T>::JsonTValue(const T& val) {
	_value = val;
}

template<typename T>
JsonTValue<T>::JsonTValue(const JsonTValue<T>& rhs) : JsonValue(rhs) {
	_value = rhs._value;
}

template<typename T>
JsonTValue<T>& JsonTValue<T>::operator= (const JsonTValue<T>& rhs) {
	JsonValue::operator=(rhs);
	_value = rhs._value;
	return *this;
}

template<typename T>
JsonTValue<T>::~JsonTValue() {

}

template<typename T>
void JsonTValue<T>::setValue(const T& val) {
	_value = val;
}

template<typename T>
T JsonTValue<T>::getValue() const {
	return _value;
}

// restrict typenames to standard JSON values
template class JsonTValue<bool>;
template class JsonTValue<int>;
template class JsonTValue<double>;
template class JsonTValue<std::string>;
template class JsonTValue<std::vector<JsonValue>>;




JsonObject::JsonObject() : JsonValue(JSON_OBJECT) {

}

JsonObject::JsonObject(const JsonObject& rhs) : JsonValue(rhs) {
	_values = rhs._values;
}

JsonObject& JsonObject::operator= (const JsonObject& rhs) {
	JsonValue::operator=(rhs);
	_values = rhs._values;
	return *this;
}

JsonObject::~JsonObject() {

}

bool JsonObject::add(const std::string& name, const JsonValue& value) {
	return _values.emplace(name, value).second;
}

void JsonObject::setValue(const std::string& name, const JsonValue& value) {
	auto it = _values.find(name);
	if (it != _values.end()) {
		it->second = value;
	}
}

JsonValue JsonObject::getValue(const std::string& name) const {
	auto it = _values.find(name);
	if (it != _values.end()) {
		return it->second;
	}
	return JsonValue();
}

JsonValue JsonObject::getValue(const char* name) const {
	return getValue(std::string(name));
}
