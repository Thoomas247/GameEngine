#include "Skeleton.h"

Skeleton::Skeleton()
{
}

Skeleton::Skeleton(const std::vector<Joint>& joints, const std::map<std::string, Animation>& animations)
{
	m_Joints = joints;
	m_Animations = animations;
}

void Skeleton::onUpdate(const float& deltaTime)
{
	// TODO: Animate
}