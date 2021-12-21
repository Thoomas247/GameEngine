#include "AnimationManager.h"

std::vector<std::weak_ptr<Skeleton>> AnimationManager::Skeletons;

void AnimationManager::Update(const float& deltaTime)
{
	for (int i = 0; i < AnimationManager::Skeletons.size(); i++)
	{
		std::weak_ptr<Skeleton>& weakSkeleton = AnimationManager::Skeletons[i];
		std::shared_ptr<Skeleton> skeleton = weakSkeleton.lock();

		// check if skeleton still exists and erase if not
		if (skeleton.get() == nullptr)
		{
			AnimationManager::Skeletons.erase(AnimationManager::Skeletons.begin() + i);
			continue;
		}

		// TODO: handle animation update
	}
}
