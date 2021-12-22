#ifndef RENDER_DATA
#define RENDER_DATA

#include "glm/glm.hpp"

#include "Material.h"
#include "../renderer/Shader.h"

struct RenderData
{
	glm::mat4* Transform = nullptr;	// pointer to GameObject::m_GlobalTransform
	Material RenderMaterial;
	Shader ShaderProgram;

	RenderData()
	{
	}

	RenderData(const Material& material, const Shader& shader)
	{
		RenderMaterial = material;
		ShaderProgram = shader;
	}
};

#endif // !RENDER_DATA