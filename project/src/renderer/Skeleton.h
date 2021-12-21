#ifndef SKELETON
#define SKELETON

#include <vector>
#include <map>
#include <string>

#include "../structs/Joint.h"
#include "../structs/Animation.h"

class Skeleton
{
private:
	std::vector<Joint> m_Joints;
	std::map<std::string, Animation> m_Animations;
	Animation* m_CurrentAnimation = nullptr;

public:
	Skeleton();
	Skeleton(const std::vector<Joint>& joints, const std::map<std::string, Animation>& animations);

	void UpdateAnimation(const float& deltaTime);
	void SetAnimation(const std::string& name);

};

#endif // !SKELETON


