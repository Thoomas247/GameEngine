#ifndef DATA
#define DATA

#include <memory>
#include <vector>

#include "Input.h"

struct Data
{
	Input Input;
	glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
};

#endif // !DATA
