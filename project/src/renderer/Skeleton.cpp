#include "Skeleton.h"

#include "../managers/AnimationManager.h"

// PUBLIC
Skeleton::Skeleton()
{
}

Skeleton::Skeleton(const std::vector<Joint>& joints, const std::map<std::string, Animation>& animations)
{
	m_Joints = joints;
	m_Animations = animations;
	//AnimationManager::Skeletons.push_back(std::shared_ptr<Skeleton>(this));
}

void Skeleton::UpdateAnimation(const float& deltaTime)
{

}

void Skeleton::SetAnimation(const std::string& name)
{
	m_CurrentAnimation = &m_Animations[name];
}
