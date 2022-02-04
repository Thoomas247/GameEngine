#include "Skeleton.h"

Skeleton::Skeleton()
{
	m_Name = "Skeleton";
}

Skeleton::Skeleton(const std::string& name, const std::vector<Joint>& joints, const std::map<std::string, Animation>& animations)
{
	NumJoints = (unsigned int)joints.size();

	m_Name = name;
	m_Joints = joints;
	m_Animations = animations;
}

void Skeleton::onUpdate(const float&)
{
	// TODO: Animate
}