#pragma once

#include "Component.h"

#include "../renderer/VertexArray.h"
#include "../renderer/Shader.h"
#include "../structs/Material.h"

class MeshComponent : public Component
{
private:
	VertexArray m_VertexArray;
	Shader m_Shader;
	Material m_Material;
	glm::mat4 m_ModelMat;

public:
	/*
	MeshComponent(const uint64_t& entityID)
		: Component(entityID)
	{
	}
	*/

	MeshComponent(Entity* entity, const VertexArray& vertexArray, const Shader& shader, const Material& material)
		: Component(entity)
	{
		m_VertexArray = vertexArray;
		m_Shader = shader;
		m_Material = material;
		m_ModelMat = glm::mat4(1.0f);
	}

	VertexArray* GetVertexArray() { return &m_VertexArray; }
	Shader* GetShader() { return &m_Shader; }
	Material* GetMaterial() { return &m_Material; }

	void SetModelMat(const glm::mat4& mat) { m_ModelMat = mat; }
	const glm::mat4 GetModelMat() { return m_ModelMat; }

};

