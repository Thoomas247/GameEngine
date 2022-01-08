#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "../core/GameObject.h"

#include "../structs/Joint.h"
#include "../structs/Animation.h"

class Skeleton : public GameObject
{
public:
	Animation* m_CurrentAnimation = nullptr;

private:
	std::vector<Joint> m_Joints;
	std::map<std::string, Animation> m_Animations;

public:
	Skeleton();
	Skeleton(const std::vector<Joint>& joints, const std::map<std::string, Animation>& animations);

private:
	void onUpdate(const float& deltaTime) override;
};