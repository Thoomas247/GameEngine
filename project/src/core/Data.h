#ifndef DATA
#define DATA

#include "glm/glm.hpp"

#include "Input.h"

struct Data
{
	Input Input;
	glm::mat4  ViewProjectionMatrix = glm::mat4(1.0f);
};

#endif // !DATA

