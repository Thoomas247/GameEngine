#pragma once

#include <map>
#include <string>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

class GameObject
{
public:
	glm::vec3 LocalPosition = glm::vec3(0.0f);
	glm::quat LocalRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 LocalScale = glm::vec3(1.0f);

	glm::mat4 GlobalTransform = glm::mat4(1.0f);

	std::map<std::string, std::shared_ptr<GameObject>> Children;

protected:
	glm::vec3 m_LastLocalPosition = glm::vec3(0.0f);
	glm::quat m_LastLocalRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 m_LastLocalScale = glm::vec3(1.0f);

	glm::mat4 m_LocalTransform = glm::mat4(1.0f);
	

public:
	void SetUp();
	void Update(const float& deltaTime, const glm::mat4& parentTransform = glm::mat4(1.0f));

	void AddChild(const std::string& name, const std::shared_ptr<GameObject>& object);
	std::shared_ptr<GameObject> GetChild(const std::string& path);

private:
	void calcTransforms(const glm::mat4& parentTransform);

	virtual void onSetUp();
	virtual void onUpdate(const float& deltaTime);	// Implemented by sub-classes
};