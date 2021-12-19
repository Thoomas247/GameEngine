#ifndef DATA
#define DATA

#include "glm/glm.hpp"

#include "Input.h"

// Data which needs to be shared accross the whole engine
namespace Data
{
	extern Input Inputs;
	extern glm::mat4 ViewProjectionMatrix;
}

#endif // !DATA
