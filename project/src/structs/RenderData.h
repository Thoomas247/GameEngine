#ifndef RENDER_DATA
#define RENDER_DATA

#include <memory>

#include "glm/glm.hpp"

#include "Material.h"
#include "../renderer/Shader.h"

struct RenderData
{
	glm::mat4* Transform = nullptr;	// pointer to GameObject::m_GlobalTransform
	std::shared_ptr<Material> RenderMaterial;
	std::shared_ptr<Shader> ShaderProgram;

	RenderData()
	{
	}

	RenderData(const std::shared_ptr<Material>& material, const std::shared_ptr<Shader>& shader)
	{
		RenderMaterial = material;
		ShaderProgram = shader;
	}
};

#endif // !RENDER_DATA