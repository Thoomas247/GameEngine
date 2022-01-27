#pragma once

#include <vector>

#include "glm/glm.hpp"

/*
	Mesh class
	Base class for anything which needs to be rendered
	on screen
	Should not be inherited from
*/

#include "../core/GameObject.h"
#include "VertexArray.h"
#include "Skeleton.h"
#include "../structs/Material.h"
#include "Shader.h"

class Mesh : public GameObject
{
private:
	std::shared_ptr<VertexArray> m_VertexArray;
	std::shared_ptr<Skeleton> m_Skeleton;
	std::shared_ptr<Material> m_Material;
	std::shared_ptr<Shader> m_Shader;

public:
	Mesh();
	Mesh(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<Skeleton> skeleton, std::shared_ptr<Material> material, std::shared_ptr<Shader> shader, glm::mat4 transform = glm::mat4(1.0f));

	VertexArray* GetVertexArray() const { return m_VertexArray.get(); }
	void SetVertexArray(const std::shared_ptr<VertexArray>& vertexArray) { m_VertexArray = vertexArray; }

	Skeleton* GetSkeleton() const { return m_Skeleton.get(); }
	void SetSkeleton(const std::shared_ptr<Skeleton>& skeleton) { m_Skeleton = skeleton; }

	Material* GetMaterial() const { return m_Material.get(); }
	void SetMaterial(const std::shared_ptr<Material>& material) { m_Material = material; }

	Shader* GetShader() const { return m_Shader.get(); }
	void SetShader(const std::shared_ptr<Shader>& shader) { m_Shader = shader; }

private:
	void onUpdate(const float& deltaTime) override;
};