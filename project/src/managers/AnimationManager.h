#ifndef ANIMATION_MANAGER
#define ANIMATION_MANAGER

#include <vector>
#include <memory>

#include "../renderer/Skeleton.h"

namespace AnimationManager
{
	extern std::vector<std::weak_ptr<Skeleton>> Skeletons;

	void Update(const float& deltaTime);
}

#endif // !ANIMATION_MANAGER