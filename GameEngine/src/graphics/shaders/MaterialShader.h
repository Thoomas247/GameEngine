#pragma once

struct CachedShader
{
	ShaderType Type;
	std::string Path;

	CachedShader(const ShaderType& type, const std::string& path)
	{
		Type = type;
		Path = path;
	}
};

class MaterialShader
{
private:
	static const unsigned int NOT_COMPILED = (unsigned int)(-1);
	static constexpr auto DEFAULT_PATH = "assets/shaders/PBRShader.glsl";
	static constexpr auto MODEL_MAT_UNIFORM_NAME = "model_mat";

	unsigned int m_GLID = NOT_COMPILED;

	std::string m_GlslPath;
	std::vector<UniformBuffer> m_UniformBuffers;

public:
	MaterialShader(const std::string& glslPath = DEFAULT_PATH, const std::vector<UniformBuffer>& savedBuffers = std::vector<UniformBuffer>())
	{
		m_GlslPath = glslPath;
		Load(Compile(), savedBuffers);
	}

	/// <summary>
	/// Compile the shader to spirv.
	/// </summary>
	/// <returns>A vector of filepaths where the cached spirv code can be found</returns>
	std::vector<CachedShader> Compile();
	/// <summary>
	/// Loads spirv into opengl and sets the uniforms to the passed values.
	/// </summary>
	/// <param name="savedUniforms">Previously saved uniform values</param>
	void Load(const std::vector<CachedShader>& spirvFiles, const std::vector<UniformBuffer>& savedBuffers = std::vector<UniformBuffer>());
	/// <summary>
	/// Activate this shader for rendering.
	/// </summary>
	void Activate();
	/// <summary>
	/// Unload this shader from the GPU.
	/// </summary>
	void Unload();

private:
	std::string splitShader(const std::string& shaderString, const std::string& shaderType);

	std::vector<uint32_t> toSpirV(const std::string& shaderString, const shaderc_shader_kind& type);
	std::vector<uint32_t> loadSpirvFileContents(const std::string& absolutePath);

	void prepareBuffers(const std::vector<uint32_t>& shaderWords);

	std::string loadGlslFileContents(const std::string& absolutePath);

	unsigned int openglCompileShaderFromSpirV(const std::vector<uint32_t>& spirvSource, const int& shaderType);
	void openglCheckCompileErrors(const unsigned int& shader, const std::string& type);
};