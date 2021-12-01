#ifndef SHADER
#define SHADER

#include <string>

#include "glm/glm.hpp"

struct Shader 
{
	unsigned int GLID = 0;	// opengl ID of shader after compilation
	std::string VertexPath;
	std::string FragmentPath;

	Shader() {}

	Shader(unsigned int id, std::string vertexPath, std::string fragmentPath) {
		GLID = id;
		VertexPath = vertexPath;
		FragmentPath = fragmentPath;
	}
};

namespace Shader_f 
{
	Shader LoadShader(const std::string& vertexPath, const std::string& fragmentPath);

	void SetBool(const Shader& shader, const std::string& assetName, bool value);
	void SetInt(const Shader& shader, const std::string& assetName, int value);
	void SetFloat(const Shader& shader, const std::string& assetName, float value);
	void SetVec2(const Shader& shader, const std::string& assetName, const glm::vec2& value);
	void SetVec3(const Shader& shader, const std::string& assetName, const glm::vec3& value);
	void SetVec4(const Shader& shader, const std::string& assetName, const glm::vec4& value);
	void SetMat2(const Shader& shader, const std::string& assetName, const glm::mat2& mat);
	void SetMat3(const Shader& shader, const std::string& assetName, const glm::mat3& mat);
	void SetMat4(const Shader& shader, const std::string& assetName, const glm::mat4& mat);
}

void checkCompileErrors(const unsigned int& shader, const std::string& type);

#endif // !SHADER
