#include "precompiled.h"
#include "MaterialShader.h"

/* -- PUBLIC -- */

void MaterialShader::Compile()
{
	// delete previous shader, if any
	if (m_GLID != 0)
	{
		Unload();
	}

	std::string shaderString = loadFileContents(m_FilePath);
	
	// split shaders
	std::string vertCode = splitShader(shaderString, "vertex");
	std::string fragCode = splitShader(shaderString, "fragment");

	// compile shaders to spirv
	std::vector<uint32_t> vertexResult = toSpirV(vertCode, shaderc_vertex_shader);
	std::vector<uint32_t> fragmentResult = toSpirV(fragCode, shaderc_fragment_shader);
	// TODO: Cache binary on disk

	// get uniforms
	getUniforms(vertexResult);
	getUniforms(fragmentResult);

	// compile shaders to OpenGL
	unsigned int vertex;
	std::vector<char> vBuffer(vertexResult.begin(), vertexResult.end());
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderBinary(1, &vertex, GL_SHADER_BINARY_FORMAT_SPIR_V, vBuffer.data(), vBuffer.size());
	checkCompileErrors(vertex, "VERTEX");

	// fragment Shader
	unsigned int fragment;
	std::vector<char> fBuffer(fragmentResult.begin(), fragmentResult.end());
	fragment = glCreateShader(GL_VERTEX_SHADER);
	glShaderBinary(1, &fragment, GL_SHADER_BINARY_FORMAT_SPIR_V, fBuffer.data(), fBuffer.size());
	checkCompileErrors(fragment, "FRAGMENT");

	// shader Program
	unsigned int glID = glCreateProgram();
	glAttachShader(glID, vertex);
	glAttachShader(glID, fragment);
	glLinkProgram(glID);
	checkCompileErrors(glID, "PROGRAM");

	// delete the individual shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	m_GLID = glID;

	m_ModelMatLocation = glGetUniformLocation(m_GLID, MODEL_MAT_UNIFORM_NAME);

	if (m_ModelMatLocation == -1)
	{
		LOG_ERROR("SHADER::Shader's model matrix uniform cannot be found! It must be named " + std::string(MODEL_MAT_UNIFORM_NAME));
	}
	
}

void MaterialShader::Activate()
{
	if (m_GLID == NOT_COMPILED)
	{
		LOG_ERROR("MATERIAL_SHADER::Shader hasn't been compiled!");
	}
	glUseProgram(m_GLID);

	for (Uniform& uniform : m_Uniforms)
	{
		uniform.Set();
	}
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

std::vector<uint32_t> MaterialShader::toSpirV(const std::string& shaderString, const shaderc_shader_kind& type)
{
	shaderc::Compiler compiler = shaderc::Compiler();
	shaderc::CompileOptions compileOptions;
	compileOptions.SetTargetEnvironment(shaderc_target_env_opengl, 0);
	compileOptions.SetAutoBindUniforms(true);
	compileOptions.SetAutoMapLocations(true);
	compileOptions.SetOptimizationLevel(shaderc_optimization_level_performance);

	shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(shaderString, type, "Shader");
	if (result.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		LOG_ERROR("SHADER::Shader compilation failed! (" + result.GetErrorMessage() + ")");
	}

	return std::vector<uint32_t>(result.begin(), result.end());
}

void MaterialShader::getUniforms(const std::vector<uint32_t>& shaderWords)
{
	spirv_cross::Compiler compiler = spirv_cross::Compiler(shaderWords);
	spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	for (const auto& resource : resources.uniform_buffers)
	{
		const auto& bufferType = compiler.get_type(resource.base_type_id);
	}
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
