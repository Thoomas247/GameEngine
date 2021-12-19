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
	ShaderProgram = shader;
	makeTri();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Shader& shader)
{
	ShaderProgram = shader;
	NumElements = indices.size();
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

	NumElements = 3;

	createMeshBuffers(vertices, indices);
}

void Mesh::createMeshBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VAO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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

void Mesh::onUpdate(Data& data, const float& deltaTime)
{
}

void Mesh::onDraw(const Data& data)
{
	glUseProgram(ShaderProgram.GetGLID());

	ShaderProgram.SetMat4("model", GlobalTransform);
	ShaderProgram.SetMat4("view_projection", data.ViewProjectionMatrix);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, NumElements, GL_UNSIGNED_INT, 0);	// we set up the EBO, so no need to pass indices
	glBindVertexArray(0);	// unbind when done
}