#ifndef DATA
#define DATA

#include <vector>
#include <memory>

#include "glm/glm.hpp"

#include "Input.h"
#include "GameObject.h"

// Data which needs to be shared accross the whole engine
namespace Data
{
	extern Input Inputs;
	extern glm::mat4 ViewProjectionMatrix;
}

#endif // !DATA
