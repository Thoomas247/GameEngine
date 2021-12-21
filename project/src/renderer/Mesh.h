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
#include "../structs/Vertex.h"
#include "../structs/Material.h"
#include "Shader.h"
#include "RenderData.h"
#include "Skeleton.h"

class Mesh : public GameObject
{
public:
	unsigned int m_VAO, m_VBO, m_EBO;
	unsigned int m_NumElements;

	Shader m_ShaderProgram;
	Material m_Material;

private:
	std::shared_ptr<Skeleton> m_Skeleton;

public:
	Mesh();
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Shader& shader, const Material& material, const std::shared_ptr<Skeleton>& skeleton);

private:
	void createMeshBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

	void onUpdate(const float& deltaTime) override;
};

#endif // !MESH
