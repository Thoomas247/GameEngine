#include "GameObject.h"

// PUBLIC
void GameObject::SetUp()
{
	onSetUp();

	for (auto& [name, object] : m_Children)
	{
		object->SetUp();
	}
}

void GameObject::Update(const float& deltaTime, const glm::mat4& parentTransform)
{
	onUpdate(deltaTime);
	calcTransforms(parentTransform);

	for (auto& [name, object] : m_Children)
	{
		object->Update(deltaTime, m_GlobalTransform);
	}
}

void GameObject::AddChild(const std::string& name, const std::shared_ptr<GameObject>& object)
{
	m_Children[name] = object;
}

std::shared_ptr<GameObject> GameObject::GetChild(const std::string& path)
{
	size_t index = path.find_first_of("/");
	std::string name = path.substr(0, index);

	const std::map<std::string, std::shared_ptr<GameObject>>::iterator it = m_Children.find(name);
	if (it != m_Children.end())
	{
		if (index == std::string::npos)
		{
			return it->second;
		}

		std::string newPath = path.substr(index);
		return it->second->GetChild(newPath);
	}

	return NULL;
}

// PRIVATE
void GameObject::calcTransforms(const glm::mat4& parentTransform)
{
	if (m_LocalPosition == m_LastLocalPosition && m_LocalRotation == m_LastLocalRotation && m_LocalScale == m_LastLocalScale)
	{
		m_GlobalTransform = parentTransform * m_LocalTransform;
		return;
	}

	m_LocalTransform = glm::translate(glm::mat4(1.0f), m_LocalPosition) * glm::mat4_cast(m_LocalRotation) * glm::scale(glm::mat4(1.0f), m_LocalScale);

	m_LastLocalPosition = m_LocalPosition;
	m_LastLocalRotation = m_LocalRotation;
	m_LastLocalScale = m_LocalScale;

	m_GlobalTransform = parentTransform * m_LocalTransform;
}

void GameObject::onSetUp()
{
	// Does nothing by default
}

void GameObject::onUpdate(const float& deltaTime)
{
	// Does nothing by default
}