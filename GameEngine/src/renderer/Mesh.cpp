#include "Mesh.h"

#include "glad/gl.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"

// PUBLIC
Mesh::Mesh()
{
}

Mesh::Mesh(const RenderData& renderData, std::shared_ptr<MeshData> meshData, std::shared_ptr<Skeleton> skeleton, glm::mat4 transform)
{
	m_RenderData = renderData;
	m_MeshData = meshData;
	m_Skeleton = skeleton;

	m_LocalTransform = transform;

	// TODO: write own decompose function
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_LocalTransform, m_LocalScale, m_LocalRotation, m_LocalPosition, skew, perspective);

	//m_LocalRotation = glm::conjugate(m_LocalRotation);

	m_RenderData.m_Transform = &m_GlobalTransform;
	m_MeshData->m_RenderData.push_back(&m_RenderData);
}

// PRIVATE
void Mesh::onUpdate(const float& deltaTime)
{
}