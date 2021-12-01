#ifndef MESH
#define MESH

#include <vector>

#include "Shader.h"
#include "Vertex.h"

struct Mesh
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	unsigned int VAO, VBO, EBO;

	Shader ShaderProgram;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Shader shader) {
		Vertices = vertices;
		Indices = indices;
		ShaderProgram = shader;
	}
};

namespace Mesh_f {
	Mesh LoadTri(const Shader& shader);
}

#endif // !MESH

