#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/gl.h"

#include "../core/ProjectManager.h"

// PUBLIC
Shader::Shader()
{
	loadShader(ProjectManager::ProjectPath + ProjectManager::DefaultShadersPath + "Base.vert",
		ProjectManager::ProjectPath + ProjectManager::DefaultShadersPath + "Base.frag");

	m_UniformLocationCache.reserve(20);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	loadShader(vertexPath, fragmentPath);

	m_UniformLocationCache.reserve(10);
}

void Shader::SetBool(const std::string& uniformName, const bool& value)
{
	glUniform1i(getUniformLocation(uniformName), (int)value);
}

void Shader::SetInt(const std::string& uniformName, const int& value)
{
	glUniform1i(getUniformLocation(uniformName), value);
}

void Shader::SetFloat(const std::string& uniformName, const float& value)
{
	glUniform1f(getUniformLocation(uniformName), value);
}

void Shader::SetVec2(const std::string& uniformName, const glm::vec2& value)
{
	glUniform2fv(getUniformLocation(uniformName), 1, &value[0]);
}

void Shader::SetVec3(const std::string& uniformName, const glm::vec3& value)
{
	glUniform3fv(getUniformLocation(uniformName), 1, &value[0]);
}

void Shader::SetVec4(const std::string& uniformName, const glm::vec4& value)
{
	glUniform4fv(getUniformLocation(uniformName), 1, &value[0]);
}

void Shader::SetMat2(const std::string& uniformName, const glm::mat2& mat)
{
	glUniformMatrix2fv(getUniformLocation(uniformName), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string& uniformName, const glm::mat3& mat)
{
	glUniformMatrix3fv(getUniformLocation(uniformName), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string& uniformName, const glm::mat4& mat)
{
	glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, &mat[0][0]);
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
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
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

int Shader::getUniformLocation(const std::string& uniformName)
{
	auto locationIterator = m_UniformLocationCache.find(uniformName);
	if (locationIterator != m_UniformLocationCache.end())
	{
		return locationIterator->second;
	}
	int location = glGetUniformLocation(m_GLID, uniformName.c_str());
	m_UniformLocationCache[uniformName] = location;
	return location;
}

void Shader::checkCompileErrors(const unsigned int& shader, const std::string& type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER::ERROR::Shader compilation failed!" << "\n" << type << ": " << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER::ERROR::Shader linking failed!" << "\n" << type << ": " << infoLog << std::endl;
		}
	}
}