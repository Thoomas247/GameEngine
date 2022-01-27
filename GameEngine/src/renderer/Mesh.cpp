#include "Mesh.h"

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

	SetLocalTransform(transform);
}

// PRIVATE
void Mesh::onUpdate(const float& deltaTime)
{
	Renderer::DrawList.push_back(this);
}