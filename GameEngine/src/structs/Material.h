#pragma once

#include "glm/glm.hpp"

struct Material
{
	glm::vec4 BaseColorFactor;
	glm::vec3 EmissiveFactor;
	float MetallicFactor;
	float RoughnessFactor;

	int BaseColorTexture;
	int EmissiveTexture;
	int MetallicRoughnessTexture;	// metalness in blue channel, roughness in green channel
	int NormalTexture;
	int OcclusionTexture;

	Material(const glm::vec4& baseF, const glm::vec3& emissiveF, const float& metallicF, const float& roughnessF, const int& baseT, 
		const int& emissiveT, const int& metallicRoughnessT, const int& normalT, const int& occlusionT)
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