#ifndef ANIMATION
#define ANIMATION

#include <map>

#include "glm/glm.hpp"

struct Animation
{
	std::map<float, std::map<int, glm::mat4>> Keyframes;

	Animation()
	{
	}

	Animation(const std::map<float, std::map<int, glm::mat4>>& keyframes)
	{
		Keyframes = keyframes;
	}
};

#endif // !ANIMATION
