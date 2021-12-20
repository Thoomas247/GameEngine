#include "Root.h"

// PUBLIC
void Root::SetUp()
{
	for (auto& [name, object] : m_Children)
	{
		object->SetUp();
	}
}

void Root::Update(const float& deltaTime)
{
	for (auto& [name, object] : m_Children)
	{
		object->Update(deltaTime);
	}
}

void Root::AddChild(const std::string& name, const std::shared_ptr<GameObject>& object)
{
	m_Children[name] = object;
}

std::shared_ptr<GameObject> Root::GetChild(const std::string& path)
{
	size_t index = path.find_first_of("/");
	std::string name = path.substr(0, index);

	const std::unordered_map<std::string, std::shared_ptr<GameObject>>::iterator it = m_Children.find(name);
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