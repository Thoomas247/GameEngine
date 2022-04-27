#pragma once

class MaterialShader
{
public:
	VulkanPipeline Pipeline;

private:
	std::vector<CachedShaderInfo> m_SpirvFileInfos;
	std::vector<SpirvCodeInfo> m_SpirvCode;
	//std::vector<UniformBuffer> m_UniformBuffers;

	VulkanDescriptorPool m_DescriptorPool;

public:
	void Compile(const std::string& glslPath);

private:
	/// <summary>
	/// Compiles the glsl file at the given path into spirv files.
	/// Populates m_SpirvFileInfos with data about the spirv files it creates/retrieves.
	/// </summary>
	/// <param name="glslPath"></param>
	void compileGlsl(const std::string& glslPath);
	/// <summary>
	/// Loads the spirv files described by m_SpirvFileInfos and stores them in m_SpirvCode.
	/// </summary>
	void loadSpirvFiles();
	/// <summary>
	/// Reflects on the shaders in m_SpirvCode to retrieve their uniforms.
	/// Populates m_UniformBuffers.
	/// </summary>
	void reflect();
};

