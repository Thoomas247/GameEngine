#ifndef MESH
#define MESH

#include <vector>

#include "glm/glm.hpp"

/*
	Mesh class
	Base class for anything which needs to be rendered
	on screen
*/

#include "../game/GameObject.h"
#include "../core/Data.h"
#include "Shader.h"
#include "Vertex.h"

class Mesh : public GameObject
{
private:
	unsigned int VAO, VBO, EBO;
	unsigned int NumElements;

	Shader ShaderProgram;

public:
	Mesh();
	Mesh(const Shader& shader);
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Shader& shader);

private:
	void createMeshBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	void makeTri();

	void onUpdate(Data& data, const float& deltaTime) override;
	void onDraw(const Data& data) override;
};

#endif // !MESH
