#pragma once

#include <string>
#include <memory>
#include <vector>

#include "GameObject.h"

class Scene
{
private:
	std::string m_Name = "";
	std::string m_Path = "";

	std::vector<std::shared_ptr<GameObject>> m_GameObjects;

public:
	Scene(const std::string& name, const std::string& path);
	Scene(const Scene& scene);

	const std::vector<std::shared_ptr<GameObject>>* GetGameObjects() { return &m_GameObjects; }

	void AddGameObject(const std::shared_ptr<GameObject>& gameObject);

	bool Save();
	bool SaveAs(const std::string& path, const std::string& name);

	void SetUp();
	void Update(const float& deltaTime);

private:
	bool saveToFile();
};