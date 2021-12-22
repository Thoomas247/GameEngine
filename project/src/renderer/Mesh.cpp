#include "Mesh.h"

#include "glad/gl.h"
#include "glm/gtc/matrix_transform.hpp"

// PUBLIC
Mesh::Mesh()
{
}

Mesh::Mesh(const RenderData& renderData, std::shared_ptr<MeshData> meshData, std::shared_ptr<Skeleton> skeleton)
{
	m_RenderData = renderData;
	m_MeshData = meshData;
	m_Skeleton = skeleton;

	m_RenderData.Transform = &m_GlobalTransform;
	m_MeshData->RenderData.push_back(&m_RenderData);
}

// PRIVATE
void Mesh::onUpdate(const float& deltaTime)
{
}