#ifndef WORLD_ROOT
#define WORLD_ROOT

#include <unordered_map>
#include <string>
#include <memory>
#include <future>

#include "GameObject.h"
#include "Data.h"

class Root
{
private:
	// threading:
	std::vector<std::future<void>> m_Futures;

	std::unordered_map<std::string, std::shared_ptr<GameObject>> m_Children;

public:
	void SetUp();
	void Update(const float& deltaTime);

	void AddChild(const std::string& name, const std::shared_ptr<GameObject>& object);
	std::shared_ptr<GameObject> GetChild(const std::string& path);
};

#endif //!WORLD_ROOT