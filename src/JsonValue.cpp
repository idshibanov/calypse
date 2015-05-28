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

void JsonValue::print(std::ostream& out) const {
	out << "null";
}

std::ostream& operator<<(std::ostream& out, const JsonValue& v) {
	v.print(out);
	return out;
}



template<typename T>
JsonTValue<T>::JsonTValue() {
	determineType();
}

template<typename T>
JsonTValue<T>::JsonTValue(const T& val) {
	determineType();
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
void JsonTValue<T>::determineType() {
	const type_info& curr = typeid(T);
	if (curr == typeid(bool)) {
		_type = JSON_BOOLEAN;
	} else if (curr == typeid(int)) {
		_type = JSON_INTEGER;
	} else if (curr == typeid(double)) {
		_type = JSON_DOUBLE;
	} else if (curr == typeid(std::string)) {
		_type = JSON_STRING;
	} else if (curr == typeid(std::vector<shared_ptr<JsonValue>>)) {
		_type = JSON_ARRAY;
	} else {
		_type = JSON_VALUE_INVALID;
	}
}

template<typename T>
T JsonTValue<T>::getValue() const {
	return _value;
}

template<typename T>
void JsonTValue<T>::print(std::ostream& out) const {
	out << _value;
}

void JsonTValue<std::vector<shared_ptr<JsonValue>>>::print(std::ostream& out) const {
	out << '[';
	for (auto v : _value) {
		out << v << ',';
	}
	out << ']';
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const JsonTValue<T>& v) {
	v.print(out);
	return out;
}

// restrict typenames to standard JSON values
template class JsonTValue<bool>;
template class JsonTValue<int>;
template class JsonTValue<double>;
template class JsonTValue<std::string>;
template class JsonTValue<std::vector<shared_ptr<JsonValue>>>;




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

bool JsonObject::add(const std::string& name, shared_ptr<JsonValue> value) {
	return _values.emplace(name, value).second;
}

void JsonObject::setValue(const std::string& name, shared_ptr<JsonValue> value) {
	auto it = _values.find(name);
	if (it != _values.end()) {
		it->second = value;
	}
}

shared_ptr<JsonValue> JsonObject::getValue(const std::string& name) const {
	auto it = _values.find(name);
	if (it != _values.end()) {
		return it->second;
	}
	return nullptr;
}

shared_ptr<JsonValue> JsonObject::getValue(const char* name) const {
	return getValue(std::string(name));
}

const std::map<std::string, shared_ptr<JsonValue>>& JsonObject::getContents() const {
	return _values;
}

void JsonObject::print(std::ostream& out) const {
	out << '{' << endl;
	for (auto pair : _values) {
		out << pair.first << " : " << (*pair.second) << "," << endl;
	}
	out << '}';
}

std::ostream& operator<<(std::ostream& out, const JsonObject& v) {
	v.print(out);
	return out;
}