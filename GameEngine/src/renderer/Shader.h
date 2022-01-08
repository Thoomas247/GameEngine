#pragma once

#include <string>
#include <map>

#include "glm/glm.hpp"

class Shader
{
public:
	std::map <std::string, int> m_UniformLocationCache;
	unsigned int m_GLID = 0;	// opengl ID of shader after compilation
	std::string m_VertexPath;
	std::string m_FragmentPath;

	// store all uniform locations upon shader creation
	int m_ModelMatLocation = -1;
	int m_ViewMatLocation = -1;
	int m_ProjectionMatLocation = -1;
	int m_AlbedoTextureLocation = -1;
	int m_EmissiveTextureLocation = -1;
	int m_MetallicRoughnessTextureLocation = -1;
	int m_NormalTextureLocation = -1;
	int m_OcclusionTextureLocation = -1;

public:
	Shader();
	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	void SetBool(const int& uniformID, const bool& value);
	void SetInt(const int& uniformID, const int& value);
	void SetFloat(const int& uniformID, const float& value);
	void SetVec2(const int& uniformID, const glm::vec2& value);
	void SetVec3(const int& uniformID, const glm::vec3& value);
	void SetVec4(const int& uniformID, const glm::vec4& value);
	void SetMat2(const int& uniformID, const glm::mat2& mat);
	void SetMat3(const int& uniformID, const glm::mat3& mat);
	void SetMat4(const int& uniformID, const glm::mat4& mat);

private:
	void loadShader(const std::string& vertexPath, const std::string& fragmentPath);
	void setUniformLocations();
	void checkCompileErrors(const unsigned int& shader, const std::string& type);
};