#include "Shader.h"

#include <fstream>
#include <sstream>

#include "glad/gl.h"

#include "../managers/ProjectManager.h"
#include "../core/Log.h"


void checkCompileErrors(const unsigned int& shader, const std::string& type)
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

std::string loadFileContents(const std::string& path)
{
	std::string fileContents;
	std::ifstream fileStream;

	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		fileStream.open(path);

		std::stringstream stringStream;
		stringStream << fileStream.rdbuf();

		fileStream.close();
		
		fileContents = stringStream.str();
	}
	catch (std::ifstream::failure& e) {
		LOG_ERROR("SHADER::FILE_NOT_SUCCESFULLY_READ")
	}

	return fileContents;
}

// PUBLIC
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = loadFileContents(vertexPath).c_str();
	std::string fragmentString = loadFileContents(fragmentPath).c_str();

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	setUniformLocations();
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::SetBool(UniformType uniform, const bool& value)
{
	glUniform1i(m_UniformLocations[uniform], (int)value);
}

void Shader::SetInt(UniformType uniform, const int& value)
{
	glUniform1i(m_UniformLocations[uniform], value);
}

void Shader::SetFloat(UniformType uniform, const float& value)
{
	glUniform1f(m_UniformLocations[uniform], value);
}

void Shader::SetVec2(UniformType uniform, const glm::vec2& value)
{
	glUniform2fv(m_UniformLocations[uniform], 1, &value[0]);
}

void Shader::SetVec3(UniformType uniform, const glm::vec3& value)
{
	glUniform3fv(m_UniformLocations[uniform], 1, &value[0]);
}

void Shader::SetVec4(UniformType uniform, const glm::vec4& value)
{
	glUniform4fv(m_UniformLocations[uniform], 1, &value[0]);
}

void Shader::SetMat2(UniformType uniform, const glm::mat2& mat)
{
	glUniformMatrix2fv(m_UniformLocations[uniform], 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(UniformType uniform, const glm::mat3& mat)
{
	glUniformMatrix3fv(m_UniformLocations[uniform], 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(UniformType uniform, const glm::mat4& mat)
{
	glUniformMatrix4fv(m_UniformLocations[uniform], 1, GL_FALSE, &mat[0][0]);
}

// PRIVATE
void Shader::setUniformLocations()
{
	m_UniformLocations[MODEL_MAT] = glGetUniformLocation(ID, "model_mat");
	m_UniformLocations[VIEW_MAT] = glGetUniformLocation(ID, "view_mat");
	m_UniformLocations[PROJECTION_MAT] = glGetUniformLocation(ID, "projection_mat");

	m_UniformLocations[ALBEDO_TEX] = glGetUniformLocation(ID, "albedo_tex");
	m_UniformLocations[EMISSIVE_TEX] = glGetUniformLocation(ID, "emissive_tex");
	m_UniformLocations[METALLIC_ROUGHNESS_TEX] = glGetUniformLocation(ID, "metallic_roughness_tex");
	m_UniformLocations[NORMAL_TEX] = glGetUniformLocation(ID, "normal_tex");
	m_UniformLocations[OCCLUSION_TEX] = glGetUniformLocation(ID, "occlusion_tex");

	m_UniformLocations[ALBEDO_FAC] = glGetUniformLocation(ID, "albedo_fac");
	m_UniformLocations[EMISSIVE_FAC] = glGetUniformLocation(ID, "emissive_fac");
	m_UniformLocations[METALLIC_FAC] = glGetUniformLocation(ID, "metallic_fac");
	m_UniformLocations[ROUGHNESS_FAC] = glGetUniformLocation(ID, "roughness_fac");

	m_UniformLocations[IS_SELECTED] = glGetUniformLocation(ID, "is_selected");
}