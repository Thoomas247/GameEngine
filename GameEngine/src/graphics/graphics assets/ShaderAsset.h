#pragma once
#include "GraphicsAsset.h"

#include <string>

#include "glm/glm.hpp"

#include "../../core/Log.h"

class ShaderAsset : public GraphicsAsset
{
public:
	/// <summary>
	/// Default constructor should never be used.
	/// </summary>
	ShaderAsset()
		: GraphicsAsset("Invalid", "Invalid", 0)
	{
	}
	ShaderAsset(const std::string& name, const std::string& path, const unsigned int& glID)
		: GraphicsAsset(name, path, glID)
	{
	}
	~ShaderAsset();

	void Activate();

	void SetBool(const std::string& uniform, const bool& value);
	void SetInt(const std::string& uniform, const int& value);
	void SetFloat(const std::string& uniform, const float& value);
	void SetVec2(const std::string& uniform, const glm::vec2& value);
	void SetVec3(const std::string& uniform, const glm::vec3& value);
	void SetVec4(const std::string& uniform, const glm::vec4& value);
	void SetMat2(const std::string& uniform, const glm::mat2& mat);
	void SetMat3(const std::string& uniform, const glm::mat3& mat);
	void SetMat4(const std::string& uniform, const glm::mat4& mat);
};