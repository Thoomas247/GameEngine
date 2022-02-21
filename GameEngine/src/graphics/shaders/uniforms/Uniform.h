#pragma once

enum class UniformType
{
	FLOAT, VEC2, VEC3, VEC4,
	SAMPLER2D, SAMPLER3D
};

class Uniform
{
private:
	std::string m_Name;
	UniformType m_Type;
	int m_Location;			// uniform location for floats, texture binding locations (e.g. GL_TEXTURE0) for samplers
	glm::vec4 m_Value;		// only x is used for floats/samplers, xyz for vec3, all for vec4

public:
	Uniform(const std::string& name, const UniformType& type, const unsigned int& location, const glm::vec4& value)
	{
		m_Name = name;
		m_Type = type;
		m_Location = location;
		m_Value = value;
	}

	Uniform(const std::string& name, const UniformType& type, const unsigned int& location, const glm::vec3& value)
		: Uniform(name, type, location, glm::vec4(value, 0.0f))
	{
	}

	Uniform(const std::string& name, const UniformType& type, const unsigned int& location, const glm::vec2& value)
		: Uniform(name, type, location, glm::vec4(value, 0.0f, 0.0f))
	{
	}

	Uniform(const std::string& name, const UniformType& type, const unsigned int& location, const float& value)
		: Uniform(name, type, location, glm::vec4(value, 0.0f, 0.0f, 0.0f))
	{
	}

	Uniform(const std::string& name, const UniformType& type, const unsigned int& location, const int& value)
		: Uniform(name, type, location, glm::vec4(value, 0.0f, 0.0f, 0.0f))
	{
	}

	void Set();
};