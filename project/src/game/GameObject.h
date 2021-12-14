#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <unordered_map>
#include <string>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "../core/Data.h"

class GameObject
{
public:
	glm::vec3 LocalPosition = glm::vec3(0.0f);
	glm::quat LocalRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 LocalScale = glm::vec3(1.0f);

protected:
	glm::mat4 LocalTransform = glm::mat4(1.0f);

	glm::mat4 GlobalTransform = glm::mat4(1.0f);

	std::unordered_map<std::string, std::shared_ptr<GameObject>> Children;

public:
	void Update(Data& data, const float& deltaTime, const glm::mat4& parentTransform = glm::mat4(1.0f));
	void Draw(const Data& data);

	void AddChild(const std::string& name, const std::shared_ptr<GameObject>& object);
	std::shared_ptr<GameObject> GetChild(const std::string& path);

private:
	inline void calcTransforms(const glm::mat4& parentTransform);

	virtual void onUpdate(Data& data, const float& deltaTime);	// Implemented by sub-classes
	virtual void onDraw(const Data& data);	// Implemented by sub-classes
};

#endif // !GAME_OBJECT