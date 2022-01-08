#pragma once

#include <memory>

#include "glm/glm.hpp"

#include "../structs/Material.h"
#include "Shader.h"
#include "Skeleton.h"

class RenderData
{
public:
	glm::mat4* m_Transform = nullptr;	// pointer to GameObject::m_GlobalTransform in Mesh
	std::shared_ptr<Material> m_Material;
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Skeleton> m_Skeleton;

public:
	RenderData();
	RenderData(const std::shared_ptr<Material>& material, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Skeleton>& skeleton = nullptr);
};