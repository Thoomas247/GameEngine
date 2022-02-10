#pragma once

#include <vector>
#include <string>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "json/json.hpp"
using json = nlohmann::json;

#include "UUID.h"

class GameObject
{
public:
	glm::vec3 LocalPosition = glm::vec3(0.0f);
	glm::quat LocalRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 LocalScale = glm::vec3(1.0f);

protected:
	uint64_t m_UUID = UUID::GenerateUUID();;
	std::string m_Name;

	std::vector<std::shared_ptr<GameObject>> m_Children;

	glm::vec3 m_LastLocalPosition = glm::vec3(0.0f);
	glm::quat m_LastLocalRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 m_LastLocalScale = glm::vec3(1.0f);

	glm::mat4 m_LocalTransform = glm::mat4(1.0f);

	glm::mat4 m_GlobalTransform = glm::mat4(1.0f);

	bool m_Selected = false;	// temp

public:
	GameObject();
	GameObject(const std::string& name);

	void SetUp();
	void Update(const float& deltaTime, const glm::mat4& parentTransform = glm::mat4(1.0f));

	uint64_t GetUUID() { return m_UUID; }

	std::string GetName() { return m_Name; }
	void SetName(const std::string name) { m_Name = name; }

	const std::vector<std::shared_ptr<GameObject>>* GetChildren() { return &m_Children; }

	void AddChild(const std::shared_ptr<GameObject>& object);
	//std::shared_ptr<GameObject> GetChild(const std::string& name);

	glm::vec3 GetLocalRotationAsEulerAngles() const { return glm::degrees(glm::eulerAngles(LocalRotation)); }
	void SetLocalRotationFromEulerAngles(const glm::vec3& newRot) { LocalRotation = glm::quat(glm::radians(newRot)); }

	glm::mat4 GetLocalTransform() const { return m_LocalTransform; }
	void SetLocalTransform(const glm::mat4& newTransform) { decomposeMatIntoComponents(newTransform); m_LocalTransform = newTransform; }

	glm::mat4 GetGlobalTransform() const { return m_GlobalTransform; }

	bool IsSelected() const { return m_Selected; }
	void SetSelected(const bool& selected) { m_Selected = selected; }

private:
	void calcTransforms(const glm::mat4& parentTransform);
	void decomposeMatIntoComponents(const glm::mat4& matrix);

	virtual void onSetUp();
	virtual void onUpdate(const float& deltaTime);	// Implemented by sub-classes
};