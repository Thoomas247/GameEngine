#include "precompiled.h"
#include "MaterialShader.h"

/* -- PUBLIC -- */

/*
std::vector<CachedShader> MaterialShader::Compile()
{
	// notes:
	// [x] shader gets compiled from glsl to spirv
	// [x] spirv is cached
	// [] reflect on shader using spirv-cross to get uniforms
	// [x] on next load, check if glsl file has changed (eg using hash), if changed, re-compile into spirv
	// [] on shader saved, save glsl path and uniform values
	// [] on shader load, compile shader and set saved uniforms

	// load the glsl file
	std::string shaderString = loadGlslFileContents(m_GlslPath);

	// create the directory if it does not exist
	std::string shaderPath = ProjectManager::GetCachePath() + "shaders/";
	std::filesystem::create_directories(shaderPath);

	// simple hashing to detect changes
	std::hash<std::string> stringHash;
	size_t hash = stringHash(shaderString);

	std::vector<CachedShader> spirvFiles;

	// check if shader has been cached
	std::string cacheFilePath = shaderPath + std::to_string(hash) + ".cache";
	if (std::filesystem::exists(cacheFilePath))
	{
		std::ifstream in(cacheFilePath, std::ios::in | std::ios::binary);
		json j;
		in >> j;
		in.close();
		for (int type = 0; type < (int)ShaderType::NUM_TYPES; type++)
		{
			std::string typeString = ShaderUtil::GetTypeString((ShaderType)type);

			if (!j[typeString].is_null())
			{
				spirvFiles.push_back(CachedShader((ShaderType)type, j[typeString]));
			}
		}
	}
	else
	{
		shaderPath += Util::GetFileName(m_GlslPath);
		json j;

		// split shaders and compile to spirv
		for (int i = 0; i < (int)ShaderType::NUM_TYPES; i++)
		{
			ShaderType type = (ShaderType)i;

			std::string shaderCode = splitShader(shaderString, ShaderUtil::GetTypeString(type));
			if (!shaderCode.empty())
			{
				std::vector<uint32_t> shaderSpirv = toSpirV(shaderCode, ShaderUtil::GetShadercType(type));

				// save to file
				std::string spirvPath = shaderPath + ShaderUtil::GetTypeExtension(type);

				std::ofstream spirvOut(spirvPath, std::ios::out | std::ios::binary);
				spirvOut.write((char*)shaderSpirv.data(), shaderSpirv.size() * sizeof(uint32_t));
				spirvOut.flush();
				spirvOut.close();

				spirvFiles.push_back(CachedShader(type, spirvPath));
				j[ShaderUtil::GetTypeString(type)] = spirvPath;
			}
		}
		std::ofstream cacheFileOut(cacheFilePath, std::ios::out | std::ios::binary);
		cacheFileOut << j;
		cacheFileOut.close();
	}

	return spirvFiles;
}

void MaterialShader::Load(const std::vector<CachedShader>& spirvFiles, const std::vector<UniformBuffer>& savedBuffers)
{
	// unload previous shader from opengl, if any
	if (m_GLID != NOT_COMPILED)
	{
		Unload();
	}

	m_GLID = glCreateProgram();

	std::map<ShaderType, unsigned int> openglShaderIDs;

	// compile shaders to OpenGL and check for errors
	for (const CachedShader& cachedShader : spirvFiles)
	{
		std::vector<uint32_t> spirv = loadSpirvFileContents(cachedShader.Path);

		unsigned int id = openglCompileShaderFromSpirV(spirv, ShaderUtil::GetOpenglType(cachedShader.Type));
		openglCheckCompileErrors(id, ShaderUtil::GetTypeString(cachedShader.Type));

		openglShaderIDs[cachedShader.Type] = id;

		// get available uniforms directly from the spirv code
		prepareBuffers(spirv);
	}

	// link shaders and delete
	for (const auto& [type, id] : openglShaderIDs)
	{
		glAttachShader(m_GLID, id);
		glDeleteShader(id);
	}
	glLinkProgram(m_GLID);
	openglCheckCompileErrors(m_GLID, "PROGRAM");

	// TODO: set uniforms retrieved by addUniformsToVector(spirv) to values in savedBuffers
}

void MaterialShader::Activate()
{
	if (m_GLID == NOT_COMPILED)
	{
		LOG_ERROR("MATERIAL_SHADER::Shader hasn't been compiled!");
	}
	glUseProgram(m_GLID);

	for (UniformBuffer& buffer : m_UniformBuffers)
	{
		buffer.Bind();
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

*/

/* -- PRIVATE -- */

/*
std::string MaterialShader::splitShader(const std::string& shaderString, const std::string& shaderType)
{
		std::string startTag = "#start " + shaderType;
		std::string endTag = "#end " + shaderType;

		std::smatch match;

		// shader start
		std::regex startRegex = std::regex("(" + startTag + ")");
		std::regex_search(shaderString, match, startRegex);
		size_t startPos = match.position(0) + startTag.size() + 1;

		if (match.size() == 0)
		{
			return std::string();
		}

		// shader end
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

	return std::vector<uint32_t>(result.cbegin(), result.cend());
}


std::vector<uint32_t> MaterialShader::loadSpirvFileContents(const std::string& absolutePath)
{
	std::vector<uint32_t> data;
	std::ifstream fileStream(absolutePath, std::ios::in | std::ios::binary);

	size_t fileSize = std::filesystem::file_size(absolutePath);
	data.resize(fileSize / sizeof(uint32_t));

	fileStream.read((char*)data.data(), fileSize);
	fileStream.close();

	return data;
}


void MaterialShader::prepareBuffers(const std::vector<uint32_t>& shaderWords)
{
	spirv_cross::Compiler compiler = spirv_cross::Compiler(shaderWords);
	spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	for (const auto& resource : resources.uniform_buffers)
	{
		// get buffer name and type
		std::string bufferName = resource.name;
		const auto& bufferType = compiler.get_type(resource.base_type_id);

		std::vector<Uniform> uniformData;

		// loop through uniforms
		const auto& uniforms = bufferType.member_types;
		for (int i = 0; i < uniforms.size(); i++)
		{
			std::string uniformName = compiler.get_member_name(resource.base_type_id, i);
			const auto& uniformType = compiler.get_type(bufferType.member_types[i]);
		}
	}
}


std::string MaterialShader::loadGlslFileContents(const std::string& absolutePath)
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

unsigned int MaterialShader::openglCompileShaderFromSpirV(const std::vector<uint32_t>& spirvSource, const int& shaderType)
{
	unsigned int shaderID = glCreateShader(shaderType);
	glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirvSource.data(), spirvSource.size() * sizeof(uint32_t));
	glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
	return shaderID;
}

void MaterialShader::openglCheckCompileErrors(const unsigned int& shader, const std::string& type)
{
	int success = 0;
	GLchar infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("SHADER::Compilation failed!" + std::string(" - " + type + ": " + infoLog));
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("SHADER::Linking failed!" + std::string(" - " + type + ": " + infoLog));
		}
	}
}

*/
