#ifndef MESH
#define MESH

#include <vector>

#include "../external/GLIncludes.h"
#include "glm/glm.hpp"

#include "../core/Data.h"
#include "Shader.h"
#include "Vertex.h"
#include "Camera.h"

class Mesh
{
private:
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	unsigned int VAO, VBO, EBO;

	Shader ShaderProgram;

	glm::mat4 Transform = glm::mat4(1.0f);

public:
	Mesh();
	Mesh(const Shader& shader);
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Shader& shader);
	void Update(Data& data, const float& deltaTime, const glm::mat4 parentTransform = glm::mat4(1.0f));
	void MakeTri();

private:
	void createMeshBuffers();
	void drawMesh(const Data& data);
	
};

#endif // !MESH

