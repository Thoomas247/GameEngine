#include "Animator.h"

#define SKELETON_LIST_RESERVE_AMOUNT 1000

std::vector<Skeleton*> Animator::SkeletonList;

void Animator::Reserve()
{
	SkeletonList.reserve(SKELETON_LIST_RESERVE_AMOUNT);
}

void Animator::Update(const float& deltaTime)
{

}

int Animator::AddSkeleton(Skeleton* skeleton)
{
	int index = SkeletonList.size();
	SkeletonList.push_back(skeleton);
	return index;
}

void Animator::RemoveSkeleton(const int& index)
{
	SkeletonList.erase(SkeletonList.begin() + index);
}
