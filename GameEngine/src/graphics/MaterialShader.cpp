#include "precompiled.h"
#include "MaterialShader.h"
#include <regex>


/* -- PUBLIC -- */

void MaterialShader::Compile()
{
	// delete previous shader, if any
	if (m_GLID != 0)
	{
		Unload();
	}

	std::string shaderString = loadFileContents(m_FilePath);
	
	// split into vertex code
	std::string vertString = splitShader(shaderString, "vertex");

	// split into fragment code
	std::string fragString = splitShader(shaderString, "fragment");

	const char* vertexCode = vertString.c_str();
	const char* fragmentCode = fragString.c_str();

	// compile shaders
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
	unsigned int glID = glCreateProgram();
	glAttachShader(glID, vertex);
	glAttachShader(glID, fragment);
	glLinkProgram(glID);
	checkCompileErrors(glID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	m_GLID = glID;

	setUniformLocations();
}

void MaterialShader::SetModelMat4(const glm::mat4& model)
{
	glUniformMatrix4fv(m_ModelUniformLocation, 1, GL_FALSE, &model[0][0]);
}

void MaterialShader::SetViewMat4(const glm::mat4& view)
{
	glUniformMatrix4fv(m_ViewUniformLocation, 1, GL_FALSE, &view[0][0]);
}

void MaterialShader::SetProjectionMat4(const glm::mat4& projection)
{
	glUniformMatrix4fv(m_ProjectionUniformLocation, 1, GL_FALSE, &projection[0][0]);
}

void MaterialShader::Activate()
{
	if (m_GLID == NOT_COMPILED)
	{
		LOG_ERROR("MATERIAL_SHADER::Shader hasn't been compiled!");
	}
	glUseProgram(m_GLID);
}

void MaterialShader::Unload()
{
	if (m_GLID == 0)
	{
		LOG_ERROR("MATERIAL_SHADER::Trying to unload shader which hasn't been compiled!");
	}
	glDeleteProgram(m_GLID);
}


/* -- PRIVATE -- */

std::string MaterialShader::splitShader(const std::string& shaderString, const std::string& shaderType)
{
		std::string startTag = "#start " + shaderType;
		std::string endTag = "#end " + shaderType;

		std::smatch match;

		std::regex startRegex = std::regex("(" + startTag + ")");
		std::regex_search(shaderString, match, startRegex);
		size_t startPos = match.position(0) + startTag.size() + 1;

		if (match.size() == 0)
		{
			LOG_ERROR("SHADER::Shader start tag (" + startTag + ") not found!");
		}

		std::regex endRegex = std::regex("(" + endTag + ")");
		std::regex_search(shaderString, match, endRegex);
		size_t endPos = match.position(0) - 1;

		if (match.size() == 0)
		{
			LOG_ERROR("SHADER::Shader end tag (" + endTag + ") not found!");
		}

		return shaderString.substr(startPos, endPos - startPos).c_str();
}

std::string MaterialShader::loadFileContents(const std::string& absolutePath)
{
	std::string fileContents;
	std::ifstream fileStream;

	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		fileStream.open(absolutePath);

		std::stringstream stringStream;
		stringStream << fileStream.rdbuf();

		fileStream.close();

		fileContents = stringStream.str();
	}
	catch (std::ifstream::failure&) {
		LOG_ERROR("SHADER::File at path " + absolutePath + " not successfully read.");
	}

	return fileContents;
}

void MaterialShader::checkCompileErrors(const unsigned int& shader, const std::string& type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("SHADER::Shader compilation failed!" + std::string(" - " + type + ": " + infoLog));
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("SHADER::Shader linking failed!" + std::string(" - " + type + ": " + infoLog));
		}
	}
}

void MaterialShader::setUniformLocations()
{
	m_ModelUniformLocation = glGetUniformLocation(m_GLID, "model_mat");
	m_ViewUniformLocation = glGetUniformLocation(m_GLID, "view_mat");
	m_ProjectionUniformLocation = glGetUniformLocation(m_GLID, "projection_mat");
}
