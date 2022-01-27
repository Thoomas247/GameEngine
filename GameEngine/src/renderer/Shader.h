#pragma once

#include <string>
#include <map>

#include "glm/glm.hpp"

enum UniformType
{
	NONE, 
	MODEL_MAT, VIEW_MAT, PROJECTION_MAT,
	ALBEDO_TEX, EMISSIVE_TEX, METALLIC_ROUGHNESS_TEX, NORMAL_TEX, OCCLUSION_TEX,
	ALBEDO_FAC, EMISSIVE_FAC, METALLIC_FAC, ROUGHNESS_FAC,
	IS_SELECTED
};

class Shader
{
private:
	unsigned int m_ID = 0;
	std::map<UniformType, int> m_UniformLocations;

public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	unsigned int GetID() const { return m_ID; }

	void Activate();

	void SetBool(UniformType uniform, const bool& value);
	void SetInt(UniformType uniform, const int& value);
	void SetFloat(UniformType uniform, const float& value);
	void SetVec2(UniformType uniform, const glm::vec2& value);
	void SetVec3(UniformType uniform, const glm::vec3& value);
	void SetVec4(UniformType uniform, const glm::vec4& value);
	void SetMat2(UniformType uniform, const glm::mat2& mat);
	void SetMat3(UniformType uniform, const glm::mat3& mat);
	void SetMat4(UniformType uniform, const glm::mat4& mat);

private:
	void setUniformLocations();
};