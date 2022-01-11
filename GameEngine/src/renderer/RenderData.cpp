#include "RenderData.h"

RenderData::RenderData()
{
}

RenderData::RenderData(const std::shared_ptr<Material>& material, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Skeleton>& skeleton)
{
	m_Material = material;
	m_Shader = shader;
	m_Skeleton = skeleton;
}