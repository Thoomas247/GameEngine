#ifndef WORLD_ROOT
#define WORLD_ROOT

#include <unordered_map>
#include <string>
#include <memory>

#include "GameObject.h"

class Root
{
private:
	std::unordered_map<std::string, std::shared_ptr<GameObject>> Children;

public:
	void Update(const float& deltaTime);
	void Draw();

	void AddChild(const std::string& name, const std::shared_ptr<GameObject>& object);
	std::shared_ptr<GameObject> GetChild(const std::string& path);
};

#endif //!WORLD_ROOT