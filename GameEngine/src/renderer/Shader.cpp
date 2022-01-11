#include "Shader.h"

#include <fstream>
#include <sstream>

#include "glad/gl.h"

#include "../core/ProjectManager.h"
#include "../core/Log.h"

// PUBLIC
Shader::Shader()
{
	loadShader("assets/shaders/Base.vert", "assets/shaders/Base.frag");
	setUniformLocations();
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	loadShader(vertexPath, fragmentPath);
	setUniformLocations();
}

void Shader::SetBool(const int& uniformID, const bool& value)
{
	glUniform1i(uniformID, (int)value);
}

void Shader::SetInt(const int& uniformID, const int& value)
{
	glUniform1i(uniformID, value);
}

void Shader::SetFloat(const int& uniformID, const float& value)
{
	glUniform1f(uniformID, value);
}

void Shader::SetVec2(const int& uniformID, const glm::vec2& value)
{
	glUniform2fv(uniformID, 1, &value[0]);
}

void Shader::SetVec3(const int& uniformID, const glm::vec3& value)
{
	glUniform3fv(uniformID, 1, &value[0]);
}

void Shader::SetVec4(const int& uniformID, const glm::vec4& value)
{
	glUniform4fv(uniformID, 1, &value[0]);
}

void Shader::SetMat2(const int& uniformID, const glm::mat2& mat)
{
	glUniformMatrix2fv(uniformID, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const int& uniformID, const glm::mat3& mat)
{
	glUniformMatrix3fv(uniformID, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const int& uniformID, const glm::mat4& mat)
{
	glUniformMatrix4fv(uniformID, 1, GL_FALSE, &mat[0][0]);
}

// PRIVATE
void Shader::loadShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	m_VertexPath = vertexPath;
	m_FragmentPath = fragmentPath;

	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		LOG_ERROR("SHADER::FILE_NOT_SUCCESFULLY_READ")
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// shader Program
	m_GLID = glCreateProgram();
	glAttachShader(m_GLID, vertex);
	glAttachShader(m_GLID, fragment);
	glLinkProgram(m_GLID);
	checkCompileErrors(m_GLID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::checkCompileErrors(const unsigned int& shader, const std::string& type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("SHADER::Shader compilation failed!" + std::string(" - " + type + ": " + infoLog))
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("SHADER::Shader linking failed!" + std::string(" - " + type + ": " + infoLog))
		}
	}
}

void Shader::setUniformLocations()
{
	m_ModelMatLocation = glGetUniformLocation(this->m_GLID, "model");
	m_ViewMatLocation = glGetUniformLocation(this->m_GLID, "view");
	m_ProjectionMatLocation = glGetUniformLocation(this->m_GLID, "projection");
	m_AlbedoTextureLocation = glGetUniformLocation(this->m_GLID, "albedo_texture");
	m_EmissiveTextureLocation = glGetUniformLocation(this->m_GLID, "emissive_texture");
	m_MetallicRoughnessTextureLocation = glGetUniformLocation(this->m_GLID, "metallic_roughness_texture");
	m_NormalTextureLocation = glGetUniformLocation(this->m_GLID, "normal_texture");
	m_OcclusionTextureLocation = glGetUniformLocation(this->m_GLID, "occlusion_texture");
}