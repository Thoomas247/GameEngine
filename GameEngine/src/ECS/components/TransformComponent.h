#pragma once
#include "Component.h"

class TransformComponent : public Component
{
private:
	int m_ParentID;	// this is not necessarily the same as the entity's parent's transform index

	glm::vec3 m_LocalTranslation;
	glm::quat m_LocalRotation;
	glm::vec3 m_LocalScale;

	glm::mat4 m_LocalTransform;
	glm::mat4 m_GlobalTransform;

	bool m_HasChanged;

public:

	TransformComponent(Entity* entity, const glm::vec3& translation = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f));

	/// <summary>
	/// Recalculates the local transform and global transform matrices given a parent transform and returns the new global transform.
	/// </summary>
	/// <param name="parentMatrix"></param>
	glm::mat4 UpdateTransforms(const glm::mat4& parentMatrix)
	{
		if (m_HasChanged)
		{
			m_LocalTransform = glm::translate(glm::mat4(1.0f), m_LocalTranslation) * glm::mat4_cast(m_LocalRotation) * glm::scale(glm::mat4(1.0f), m_LocalScale);
			m_HasChanged = false;
		}

		m_GlobalTransform = parentMatrix * m_LocalTransform;
		return m_GlobalTransform;
	}

	int GetParentIndex() { return m_ParentID; }
	void SetParentIndex(const int& index) { m_ParentID = index; }

	void SetLocalTranslation(const glm::vec3& translation) { m_LocalTranslation = translation; m_HasChanged = true; }
	void SetLocalRotation(const glm::quat& rotation) { m_LocalRotation = rotation; m_HasChanged = true; }
	void SetLocalScale(const glm::vec3& scale) { m_LocalScale = scale; m_HasChanged = true; }

	const glm::mat4 GetLocalTransform() { return m_LocalTransform; }
	const glm::mat4 GetGlobalTransform() { return m_GlobalTransform; }

	const bool HasChanged() { return m_HasChanged; }
	void SetChanged(const bool& changed) { m_HasChanged = changed; }
};