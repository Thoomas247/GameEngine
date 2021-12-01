#ifndef PLAYER
#define PLAYER

#include "glm/glm.hpp"

#include "../renderer/Camera.h"

struct Player
{
	glm::vec3 GlobalPosition = glm::vec3(0.0f);
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
	//glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
	
	float Yaw = 0.0f;
	float Speed = 1.0f;	// m/s

	Camera Camera;
};

namespace Player_f 
{
	void Update(Player& player);	// run every frame
}

#endif // !PLAYER
