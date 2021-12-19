#include "Root.h"

// PUBLIC
void Root::Update(const float& deltaTime)
{
	for (auto& [name, object] : Children)
	{
		object->Update(deltaTime);
	}
}

void Root::Draw()
{
	for (auto& [name, object] : Children)
	{
		object->Draw();
	}
}

void Root::AddChild(const std::string& name, const std::shared_ptr<GameObject>& object)
{
	Children[name] = object;
}

std::shared_ptr<GameObject> Root::GetChild(const std::string& path)
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