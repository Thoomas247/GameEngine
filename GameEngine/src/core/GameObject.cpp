#include "GameObject.h"

// PUBLIC
void GameObject::SetUp()
{
	onSetUp();

	for (auto& [name, object] : Children)
	{
		object->SetUp();
	}
}

void GameObject::Update(const float& deltaTime, const glm::mat4& parentTransform)
{
	onUpdate(deltaTime);
	calcTransforms(parentTransform);

	for (auto& [name, object] : Children)
	{
		object->Update(deltaTime, GlobalTransform);
	}
}

void GameObject::AddChild(const std::string& name, const std::shared_ptr<GameObject>& object)
{
	Children[name] = object;
}

std::shared_ptr<GameObject> GameObject::GetChild(const std::string& path)
{
	size_t index = path.find_first_of("/");
	std::string name = path.substr(0, index);

	const std::map<std::string, std::shared_ptr<GameObject>>::iterator it = Children.find(name);
	if (it != Children.end())
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
	if (LocalPosition == m_LastLocalPosition && LocalRotation == m_LastLocalRotation && LocalScale == m_LastLocalScale)
	{
		GlobalTransform = parentTransform * m_LocalTransform;
		return;
	}

	m_LocalTransform = glm::translate(glm::mat4(1.0f), LocalPosition) * glm::mat4_cast(LocalRotation) * glm::scale(glm::mat4(1.0f), LocalScale);

	m_LastLocalPosition = LocalPosition;
	m_LastLocalRotation = LocalRotation;
	m_LastLocalScale = LocalScale;

	GlobalTransform = parentTransform * m_LocalTransform;
}

void GameObject::onSetUp()
{
	// Does nothing by default
}

void GameObject::onUpdate(const float& deltaTime)
{
	// Does nothing by default
}