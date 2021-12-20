#ifndef MESH
#define MESH

#include <vector>

#include "glm/glm.hpp"

/*
	Mesh class
	Base class for anything which needs to be rendered
	on screen
*/

#include "../core/GameObject.h"
#include "../core/Data.h"
#include "Shader.h"
#include "Vertex.h"
#include "Material.h"
#include "RenderData.h"

class Mesh : public GameObject
{
public:
	unsigned int m_VAO, m_VBO, m_EBO;
	unsigned int m_NumElements;

	Shader m_ShaderProgram;
	Material m_Material;

public:
	Mesh();
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Shader& shader, const Material& material);

private:
	void createMeshBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

	void onUpdate(const float& deltaTime) override;
};

#endif // !MESH
