#include "Mesh.h"

#include "glad/gl.h"
#include "glm/gtc/matrix_transform.hpp"

// PUBLIC
Mesh::Mesh()
{
	makeTri();
}

Mesh::Mesh(const Shader& shader)
{
	m_ShaderProgram = shader;
	makeTri();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Shader& shader)
{
	m_ShaderProgram = shader;
	m_NumElements = indices.size();
	createMeshBuffers(vertices, indices);
}

// PRIVATE
void Mesh::makeTri()
{
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f)));
	vertices.push_back(Vertex(glm::vec3(10.0f, 10.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f)));
	vertices.push_back(Vertex(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f)));

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	m_NumElements = 3;

	createMeshBuffers(vertices, indices);
}

void Mesh::createMeshBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VAO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

	glBindVertexArray(0);	// unbind VAO
}

void Mesh::onUpdate(const float& deltaTime)
{
}

void Mesh::onDraw()
{
	glUseProgram(m_ShaderProgram.GetGLID());

	m_ShaderProgram.SetMat4("model", m_GlobalTransform);
	m_ShaderProgram.SetMat4("view_projection", Data::ViewProjectionMatrix);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_NumElements, GL_UNSIGNED_INT, 0);	// we set up the EBO, so no need to pass indices
	glBindVertexArray(0);	// unbind when done
}