#pragma once

class MaterialShader
{
private:
	static const unsigned int NOT_COMPILED = (unsigned int)(-1);
	static constexpr auto DEFAULT_PATH = "assets/shaders/Default.glsl";
	static constexpr auto MODEL_MAT_UNIFORM_NAME = "model_mat";

	unsigned int m_GLID = NOT_COMPILED;

	std::string m_FilePath;
	int m_ModelMatLocation;
	std::vector<Uniform> m_Uniforms;

public:
	MaterialShader(const std::string& path = DEFAULT_PATH)
	{
		m_FilePath = path;
		Compile();
	}

	/// <summary>
	/// Compile the shader with the material constants set.
	/// </summary>
	void Compile();
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
	void getUniforms(const std::vector<uint32_t>& shaderWords);
	std::string loadFileContents(const std::string& absolutePath);
	void checkCompileErrors(const unsigned int& shader, const std::string& type);
};