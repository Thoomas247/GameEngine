#ifndef SKELETON
#define SKELETON

#include <vector>
#include <map>
#include <string>

#include "../structs/Joint.h"
#include "../structs/Animation.h"

struct Skeleton
{
	std::vector<Joint> Joints;
	std::map<std::string, Animation> Animations;
	Animation* CurrentAnimation = nullptr;

	int IndexInAnimatorList;

	Skeleton();
	~Skeleton();
	Skeleton(const std::vector<Joint>& joints, const std::map<std::string, Animation>& animations);

	void SetAnimation(const std::string& name);
};

#endif // !SKELETON
