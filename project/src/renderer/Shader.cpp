#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/glad.h"

Shader Shader_f::LoadShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	unsigned int newID;

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
	newID = glCreateProgram();
	glAttachShader(newID, vertex);
	glAttachShader(newID, fragment);
	glLinkProgram(newID);
	checkCompileErrors(newID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return Shader(newID, vertexPath, fragmentPath);
}

void Shader_f::SetBool(const Shader& shader, const std::string& assetName, bool value)
{
	glUniform1i(glGetUniformLocation(shader.GLID, assetName.c_str()), (int)value);
}

void Shader_f::SetInt(const Shader& shader, const std::string& assetName, int value)
{
	glUniform1i(glGetUniformLocation(shader.GLID, assetName.c_str()), value);
}

void Shader_f::SetFloat(const Shader& shader, const std::string& assetName, float value)
{
	glUniform1f(glGetUniformLocation(shader.GLID, assetName.c_str()), value);
}

void Shader_f::SetVec2(const Shader& shader, const std::string& assetName, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(shader.GLID, assetName.c_str()), 1, &value[0]);
}

void Shader_f::SetVec3(const Shader& shader, const std::string& assetName, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(shader.GLID, assetName.c_str()), 1, &value[0]);
}

void Shader_f::SetVec4(const Shader& shader, const std::string& assetName, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(shader.GLID, assetName.c_str()), 1, &value[0]);
}

void Shader_f::SetMat2(const Shader& shader, const std::string& assetName, const glm::mat2& mat)
{
	glUniformMatrix2fv(glGetUniformLocation(shader.GLID, assetName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader_f::SetMat3(const Shader& shader, const std::string& assetName, const glm::mat3& mat)
{
	glUniformMatrix3fv(glGetUniformLocation(shader.GLID, assetName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader_f::SetMat4(const Shader& shader, const std::string& assetName, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.GLID, assetName.c_str()), 1, GL_FALSE, &mat[0][0]);
}



void checkCompileErrors(const unsigned int& shader, const std::string& type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
