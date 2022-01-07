#ifndef JOINT
#define JOINT

#include "glm/glm.hpp"

struct Joint
{
	int ParentID;
	glm::mat4 InvBindMatrix;
	glm::mat4 PoseMatrix = glm::mat4(1.0f);

	Joint()
	{
		ParentID = -1;
		InvBindMatrix = glm::mat4(1.0f);
	}

	Joint(const int& parentID, const glm::mat4& invBindMatrix)
	{
		ParentID = parentID;
		InvBindMatrix = invBindMatrix;
	}
};

#endif // !JOINT