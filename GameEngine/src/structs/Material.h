#pragma once

#include "glm/glm.hpp"

#include "Texture.h"

struct Material
{
	glm::vec4 BaseColorFactor;
	glm::vec3 EmissiveFactor;
	float MetallicFactor;
	float RoughnessFactor;

	Texture BaseColorTexture;
	Texture EmissiveTexture;
	Texture MetallicRoughnessTexture;	// metalness in blue channel, roughness in green channel
	Texture NormalTexture;
	Texture OcclusionTexture;

	Material()
	{
		BaseColorFactor = glm::vec4(1.0f);
		EmissiveFactor = glm::vec3(0.0f);
		MetallicFactor = 0.0f;
		RoughnessFactor = 0.0f;
	}

	Material(const glm::vec4& baseF, const glm::vec3& emissiveF, const float& metallicF, const float& roughnessF, const Texture& baseT,
		const Texture& emissiveT, const Texture& metallicRoughnessT, const Texture& normalT, const Texture& occlusionT)
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