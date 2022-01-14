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
#include "../structs/BufferData.h"
#include "Skeleton.h"
#include "../structs/Material.h"
#include "Shader.h"

class Mesh : public GameObject
{
public:
	std::shared_ptr<BufferData> m_BufferData;
	std::shared_ptr<Skeleton> m_Skeleton;
	std::shared_ptr<Material> m_Material;
	std::shared_ptr<Shader> m_Shader;

public:
	Mesh();
	Mesh(std::shared_ptr<BufferData> bufferData, std::shared_ptr<Skeleton> skeleton, std::shared_ptr<Material> material, std::shared_ptr<Shader> shader, glm::mat4 transform = glm::mat4(1.0f));

private:
	void onUpdate(const float& deltaTime) override;
};