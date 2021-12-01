#include "Mesh.h"

Mesh Mesh_f::LoadTri(const Shader& shader)
{
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f)));
	vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f)));

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);


	return Mesh(vertices, indices, shader);
}

