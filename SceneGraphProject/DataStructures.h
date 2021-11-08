#pragma once
#include <string>

struct UniformData {
	std::string m_name;
	std::string m_type;
	void* m_value;
	UniformData()
		: m_name("null"), m_type("null"), m_value(nullptr) {}
	UniformData(std::string name, std::string type, void* value)
		: m_name(name), m_type(type), m_value(value) {}
};