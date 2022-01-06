#include "Animator.h"

#define SKELETON_LIST_RESERVE_AMOUNT 1000

std::vector<Skeleton*> Animator::g_SkeletonList;

void Animator::Reserve()
{
	g_SkeletonList.reserve(SKELETON_LIST_RESERVE_AMOUNT);
}

void Animator::Update(const float& deltaTime)
{
}

int Animator::AddSkeleton(Skeleton* skeleton)
{
	int index = g_SkeletonList.size();
	g_SkeletonList.push_back(skeleton);
	return index;
}

void Animator::RemoveSkeleton(const int& index)
{
	if (g_SkeletonList.size() == 0)
		return;

	g_SkeletonList[index] = g_SkeletonList[g_SkeletonList.size() - 1];
	g_SkeletonList[index]->IndexInAnimatorList = index;
	g_SkeletonList.erase(g_SkeletonList.end() - 1);
}