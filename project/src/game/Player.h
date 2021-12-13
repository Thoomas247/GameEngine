#ifndef PLAYER
#define PLAYER

#include <map>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "../core/Data.h"
#include "../renderer/Camera.h"
#include "../renderer/Mesh.h"

class Player
{
private:
	glm::vec3 LocalPosition = glm::vec3(0.0f);
	glm::quat LocalRotation = glm::quat();

	glm::mat4 GlobalTransform = glm::mat4();

	//glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);

	float Speed = 5.0f;	// m/s

	Camera Camera;
	std::map<std::string, Mesh> MeshChildren;

public:
	void Update(Data& data, const float& deltaTime, const glm::mat4& parentTransform = glm::mat4(1.0f));

	void AddChildMesh(const std::string& name, Mesh& mesh);
};

#endif // !PLAYER
