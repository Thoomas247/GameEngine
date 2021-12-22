#include "Skeleton.h"

#include "Animator.h"

Skeleton::Skeleton() 
{
}

Skeleton::~Skeleton()
{
	Animator::RemoveSkeleton(IndexInAnimatorList);
}

Skeleton::Skeleton(const std::vector<Joint>& joints, const std::map<std::string, Animation>& animations)
{
	Joints = joints;
	Animations = animations;
	IndexInAnimatorList = Animator::AddSkeleton(this);
}

void Skeleton::SetAnimation(const std::string& name)
{
	CurrentAnimation = &Animations[name];
}