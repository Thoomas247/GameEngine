#ifndef JOINT
#define JOINT

#include "glm/glm.hpp"

struct Joint
{
	int ParentID;
	glm::mat4 InvBindMatrix;
	glm::mat4 PoseMatrix;

	Joint()
	{
		ParentID = -1;
		InvBindMatrix = glm::mat4(1.0f);
		PoseMatrix = glm::mat4(1.0f);
	}
};

#endif // !JOINT