#pragma once

struct Material
{
	glm::vec4 AlbedoFactor = glm::vec4(1.0f);
	glm::vec3 EmissiveFactor = glm::vec3(0.0f);
	float MetallicFactor = 0.0f;
	float RoughnessFactor = 0.0f;

	std::string AlbedoTexture = "Default";
	std::string EmissiveTexture = "Default";
	std::string MetallicRoughnessTexture = "Default";	// metalness in blue channel, roughness in green channel
	std::string NormalTexture = "Default";
	std::string OcclusionTexture = "Default";

	Material()
	{
	}

	Material(const glm::vec4& albedoF, const glm::vec3& emissiveF, const float& metallicF, const float& roughnessF, const std::string& albedoT,
		const std::string& emissiveT, const std::string& metallicRoughnessT, const std::string& normalT, const std::string& occlusionT)
	{
		AlbedoFactor = albedoF;
		EmissiveFactor = emissiveF;
		MetallicFactor = metallicF;
		RoughnessFactor = roughnessF;

		AlbedoTexture = albedoT;
		EmissiveTexture = emissiveT;
		MetallicRoughnessTexture = metallicRoughnessT;
		NormalTexture = normalT;
		OcclusionTexture = occlusionT;
	}
};

class MaterialShader
{
private:
	static const unsigned int NOT_COMPILED = (unsigned int)(-1);
	static constexpr auto DEFAULT_PATH = "assets/shaders/Default.shader";

	unsigned int m_GLID = NOT_COMPILED;

	std::string m_FilePath;
	Material m_Material;

	unsigned int m_AlbedoLocation = 0;
	unsigned int m_EmissiveLocation = 0;
	unsigned int m_MetallicRoughnessLocation = 0;
	unsigned int m_NormalLocation = 0;
	unsigned int m_OcclusionLocation = 0;

	unsigned int m_ModelUniformLocation = 0;
	unsigned int m_ViewUniformLocation = 0;
	unsigned int m_ProjectionUniformLocation = 0;

public:
	MaterialShader(const Material& material = Material(), const std::string& path = DEFAULT_PATH)
	{
		m_FilePath = path;
		m_Material = material;
		Compile();
	}

	Material GetMaterial() { return m_Material; }
	void SetMaterial(const Material& material) { m_Material = material; }

	void SetModelMat4(const glm::mat4& model);
	void SetViewMat4(const glm::mat4& view);
	void SetProjectionMat4(const glm::mat4& projection);

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
	std::string loadFileContents(const std::string& absolutePath);
	void checkCompileErrors(const unsigned int& shader, const std::string& type);
	void setUniformLocations();
};