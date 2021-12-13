#include "Mesh.h"

// PUBLIC
Mesh::Mesh()
{

}

Mesh::Mesh(const Shader& shader)
{
	ShaderProgram = shader;
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Shader& shader)
{
	Vertices = vertices;
	Indices = indices;
	ShaderProgram = shader;

	createMeshBuffers();
}

void Mesh::Update(Data& data, const float& deltaTime, const glm::mat4 parentTransform)
{
	drawMesh(data);
}

void Mesh::MakeTri()
{
	Vertices.push_back(Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f)));
	Vertices.push_back(Vertex(glm::vec3(10.0f, 10.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f)));
	Vertices.push_back(Vertex(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f)));

	Indices.push_back(0);
	Indices.push_back(1);
	Indices.push_back(2);

	createMeshBuffers();
}

// PRIVATE
void Mesh::createMeshBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VAO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);	// unbind VAO
}

void Mesh::drawMesh(const Data& data)
{
	glUseProgram(ShaderProgram.GLID);

	Shader_f::SetMat4(ShaderProgram, "model", Transform);
	Shader_f::SetMat4(ShaderProgram, "view_projection", data.ViewProjectionMatrix);
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);	// we set up the EBO, so no need to pass indices
	glBindVertexArray(0);	// unbind when done
}

