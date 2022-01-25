#include "Mesh.h"

#include "glad/gl.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "Renderer.h"

// PUBLIC
Mesh::Mesh()
{
}

Mesh::Mesh(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<Skeleton> skeleton, std::shared_ptr<Material> material, std::shared_ptr<Shader> shader, glm::mat4 transform)
{
	m_VertexArray = vertexArray;
	m_Skeleton = skeleton;
	m_Material = material;
	m_Shader = shader;

	m_LocalTransform = transform;

	// TODO: write own decompose function
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_LocalTransform, m_LocalScale, m_LocalRotation, m_LocalPosition, skew, perspective);

}

// PRIVATE
void Mesh::onUpdate(const float& deltaTime)
{
	Renderer::DrawList.push_back(this);
}