#ifndef SHADER
#define SHADER

#include <string>

#include "../external/GLIncludes.h"
#include "glm/glm.hpp"

class Shader
{
private:
	unsigned int GLID = 0;	// opengl ID of shader after compilation
	std::string VertexPath;
	std::string FragmentPath;

public:
	Shader();
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	Shader(const unsigned int& id, const std::string& vertexPath, const std::string& fragmentPath);

	void SetBool(const std::string& uniformName, const bool& value);
	void SetInt(const std::string& uniformName, const int& value);
	void SetFloat(const std::string& uniformName, const float& value);
	void SetVec2(const std::string& uniformName, const glm::vec2& value);
	void SetVec3(const std::string& uniformName, const glm::vec3& value);
	void SetVec4(const std::string& uniformName, const glm::vec4& value);
	void SetMat2(const std::string& uniformName, const glm::mat2& mat);
	void SetMat3(const std::string& uniformName, const glm::mat3& mat);
	void SetMat4(const std::string& uniformName, const glm::mat4& mat);

private:
	void checkCompileErrors(const unsigned int& shader, const std::string& type);
};

#endif // !SHADER
