#pragma once

#include <memory>

#include "glm/glm.hpp"

#include "Texture.h"

struct Material
{
	glm::vec4 BaseColorFactor;
	glm::vec3 EmissiveFactor;
	float MetallicFactor;
	float RoughnessFactor;

	std::shared_ptr<Texture> BaseColorTexture;
	std::shared_ptr<Texture> EmissiveTexture;
	std::shared_ptr<Texture> MetallicRoughnessTexture;	// metalness in blue channel, roughness in green channel
	std::shared_ptr<Texture> NormalTexture;
	std::shared_ptr<Texture> OcclusionTexture;

	Material(const glm::vec4& baseF, const glm::vec3& emissiveF, const float& metallicF, const float& roughnessF, std::shared_ptr<Texture> baseT,
		std::shared_ptr<Texture> emissiveT, std::shared_ptr<Texture> metallicRoughnessT, std::shared_ptr<Texture> normalT, std::shared_ptr<Texture> occlusionT)
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