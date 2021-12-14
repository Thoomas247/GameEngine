#include "GameObject.h"

// PUBLIC
void GameObject::Update(Data& data, const float& deltaTime, const glm::mat4& parentTransform)
{
	onUpdate(data, deltaTime);
	calcTransforms(parentTransform);

	for (auto& [name, object] : Children)
	{
		object->Update(data, deltaTime, GlobalTransform);
	}
}

void GameObject::Draw(const Data& data)
{
	onDraw(data);
	for (auto& [name, object] : Children)
	{
		object->Draw(data);
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

	const std::unordered_map<std::string, std::shared_ptr<GameObject>>::iterator it = Children.find(name);
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
inline void GameObject::calcTransforms(const glm::mat4& parentTransform)
{
	LocalTransform = glm::scale(glm::mat4(1.0f), LocalScale);
	LocalTransform = glm::mat4_cast(LocalRotation) * LocalTransform;
	LocalTransform = glm::translate(LocalTransform, LocalPosition);

	GlobalTransform = parentTransform * LocalTransform;
}

void GameObject::onUpdate(Data& data, const float& deltaTime)
{
	// Does nothing by default
}

void GameObject::onDraw(const Data& data)
{
}