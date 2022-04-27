#include "precompiled.h"
#include "MaterialShader.h"


/* -- PUBLIC -- */

void MaterialShader::Compile(const std::string& glslPath)
{
	compileGlsl(glslPath);
	loadSpirvFiles();
	reflect();
}


/* -- PRIVATE -- */

void MaterialShader::compileGlsl(const std::string& glslPath)
{
	// load the glsl file
	std::string shaderString = ShaderUtil::LoadGlslFileContents(glslPath);

	// create the directory if it does not exist
	std::string shaderPath = ProjectManager::GetCachePath() + "shaders/";
	std::filesystem::create_directories(shaderPath);

	// simple hashing to detect changes
	std::hash<std::string> stringHash;
	size_t hash = stringHash(shaderString);

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
				m_SpirvFileInfos.push_back(CachedShaderInfo((ShaderType)type, j[typeString]));
			}
		}
	}
	else
	{
		shaderPath += Util::GetFileName(glslPath);
		json j;

		// split shaders and compile to spirv
		for (int i = 0; i < (int)ShaderType::NUM_TYPES; i++)
		{
			ShaderType type = (ShaderType)i;

			std::string shaderCode = ShaderUtil::SplitShaderStage(shaderString, type);
			if (!shaderCode.empty())
			{
				std::vector<uint32_t> shaderSpirv = ShaderUtil::ConvertToSpirv(shaderCode, type);

				// save to file
				std::string spirvPath = shaderPath + ".spv" + ShaderUtil::GetTypeExtension(type);

				std::ofstream spirvOut(spirvPath, std::ios::out | std::ios::binary);
				spirvOut.write((char*)shaderSpirv.data(), shaderSpirv.size() * sizeof(uint32_t));
				spirvOut.flush();
				spirvOut.close();

				m_SpirvFileInfos.push_back(CachedShaderInfo(type, spirvPath));
				j[ShaderUtil::GetTypeString(type)] = spirvPath;
			}
		}
		std::ofstream cacheFileOut(cacheFilePath, std::ios::out | std::ios::binary);
		cacheFileOut << j;
		cacheFileOut.close();
	}
}

void MaterialShader::loadSpirvFiles()
{
	size_t numShaders = m_SpirvFileInfos.size();

	m_SpirvCode.resize(numShaders);

	for (int i = 0; i < numShaders; i++)
	{
		// load the shader file from disk
		const CachedShaderInfo& spirvInfo = m_SpirvFileInfos.at(i);

		std::ifstream fileStream(spirvInfo.Path, std::ios::in | std::ios::binary);

		if (!fileStream.is_open())
		{
			LOG_ERROR("MATERIAL_SHADER::Failed to open spirv file!");
		}

		size_t fileSize = std::filesystem::file_size(spirvInfo.Path);
		m_SpirvCode[i].Code.resize(fileSize / sizeof(uint32_t));

		fileStream.read((char*)m_SpirvCode[i].Code.data(), fileSize);
		fileStream.close();

		m_SpirvCode[i].Type = spirvInfo.Type;
	}
}

void MaterialShader::reflect()
{
	std::vector<DescriptorPoolSize> descriptorPoolSizes;
	std::vector<DescriptorSetBinding> descriptorBindings;

	for (uint32_t i = 0; i < m_SpirvCode.size(); i++)
	{
		// reflect on the shader to retrieve its uniforms, push constants, etc...
		const std::vector<uint32_t>& shaderWords = m_SpirvCode[i].Code;
		ShaderType shaderType = m_SpirvFileInfos[i].Type;

		spirv_cross::Compiler compiler = spirv_cross::Compiler(shaderWords);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		// uniform buffers
		DescriptorPoolSize uniformBufferPoolSize{};
		uniformBufferPoolSize.Type = DescriptorType::uniform_buffer;
		uniformBufferPoolSize.Size = resources.uniform_buffers.size();
		descriptorPoolSizes.push_back(uniformBufferPoolSize);

		for (auto& uniformBuffer : resources.uniform_buffers)
		{
			DescriptorSetBinding binding{};
			binding.Binding = compiler.get_decoration(uniformBuffer.id, spv::DecorationBinding);
			binding.Type = DescriptorType::uniform_buffer;
			binding.Count = 1;	// TODO: Find way to get descriptor count using spirv_cross (count is size of array)
			binding.Stage = shaderType;

			descriptorBindings.push_back(binding);
		}

		// other...
		//DescriptorPoolSize otherPoolSize{};
		//otherPoolSize.Type = DescriptorType::other_type;
	}

	m_DescriptorPool.Init(descriptorPoolSizes, descriptorBindings);
	Pipeline.Init(m_SpirvCode, m_DescriptorPool.DescriptorSetLayout, descriptorBindings.size());

}
