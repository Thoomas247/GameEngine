#pragma once

#include "Panel.h"

#include <map>
#include <string>

#include "../core/GameObject.h"

class SceneHierarchy : public Panel
{
public:
	std::shared_ptr<GameObject> Selected = nullptr;

public:
	void Update() override;

private:
	void destroy() override;
	void addChildrenToTree(const std::map<std::string, std::shared_ptr<GameObject>>& children);
	void setChildrenAsSelected(const std::map<std::string, std::shared_ptr<GameObject>>& children);
};

