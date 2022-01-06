#ifndef ANIMATOR
#define ANIMATOR

#include <vector>
#include "Skeleton.h"

namespace Animator
{
	extern std::vector<Skeleton*> g_SkeletonList;

	void Reserve();
	void Update(const float& deltaTime);

	int AddSkeleton(Skeleton* skeleton);
	void RemoveSkeleton(const int& index);
}

#endif // !ANIMATOR
