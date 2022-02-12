#pragma once

#include "Component.h"

#include "../../graphics/GraphicsAssetManager.h"
#include "../../graphics/graphics assets/VertexArray.h"
#include "../../graphics/graphics assets/ShaderAsset.h"
#include "../../graphics/graphics assets/TextureAsset.h"


struct Material
{
	glm::vec4 BaseColorFactor;
	glm::vec3 EmissiveFactor;
	float MetallicFactor;
	float RoughnessFactor;

	TextureAsset BaseColorTexture;
	TextureAsset EmissiveTexture;
	TextureAsset MetallicRoughnessTexture;	// metalness in blue channel, roughness in green channel
	TextureAsset NormalTexture;
	TextureAsset OcclusionTexture;

	Material()
	{
		BaseColorFactor = glm::vec4(1.0f);
		EmissiveFactor = glm::vec3(0.0f);
		MetallicFactor = 0.0f;
		RoughnessFactor = 0.0f;
		BaseColorTexture			= GraphicsAssetManager::LoadTexture();
		EmissiveTexture				= GraphicsAssetManager::LoadTexture();
		MetallicRoughnessTexture	= GraphicsAssetManager::LoadTexture();
		NormalTexture				= GraphicsAssetManager::LoadTexture();
		OcclusionTexture			= GraphicsAssetManager::LoadTexture();
	}

	Material(const glm::vec4& baseF, const glm::vec3& emissiveF, const float& metallicF, const float& roughnessF, const TextureAsset& baseT,
		const TextureAsset& emissiveT, const TextureAsset& metallicRoughnessT, const TextureAsset& normalT, const TextureAsset& occlusionT)
	{
		BaseColorFactor = baseF;
		EmissiveFactor = emissiveF;
		MetallicFactor = metallicF;
		RoughnessFactor = roughnessF;

		BaseColorTexture = baseT;
		EmissiveTexture = emissiveT;
		MetallicRoughnessTexture = metallicRoughnessT;
		NormalTexture = normalT;
		OcclusionTexture = occlusionT;
	}
};


class MeshComponent : public Component
{
private:
	VertexArray m_VertexArray;
	ShaderAsset m_Shader;
	Material m_Material;
	glm::mat4 m_ModelMat;

public:
	/*
	MeshComponent(const uint64_t& entityID)
		: Component(entityID)
	{
	}
	*/

	MeshComponent(Entity* entity, const VertexArray& vertexArray, const ShaderAsset& shader, const Material& material)
		: Component(entity)
	{
		m_VertexArray = vertexArray;
		m_Shader = shader;
		m_Material = material;
		m_ModelMat = glm::mat4(1.0f);
	}

	VertexArray* GetVertexArray() { return &m_VertexArray; }
	ShaderAsset* GetShader() { return &m_Shader; }
	Material* GetMaterial() { return &m_Material; }

	void SetModelMat(const glm::mat4& mat) { m_ModelMat = mat; }
	const glm::mat4 GetModelMat() { return m_ModelMat; }

};

