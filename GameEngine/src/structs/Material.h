#pragma once

#include "glm/glm.hpp"

struct Material
{
	glm::vec4 BaseColorFactor = glm::vec4(1.0f);
	int BaseColorTexture = -1;

	float MetallicFactor = 0.0f;
	float RoughnessFactor = 0.0f;
	int MetallicRoughnessTexture = -1;	// metalness in blue channel, roughness in green channel

	glm::vec3 EmissiveFactor = glm::vec3(1.0f);
	int EmissiveTexture = -1;

	int Normaltexture = -1;
	int OcclusionTexture = -1;
};