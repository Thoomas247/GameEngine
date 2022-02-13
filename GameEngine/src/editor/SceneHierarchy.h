#pragma once

#include "Panel.h"

#include <vector>

class SceneHierarchy : public Panel
{
public:
	void Update(const float& deltaTime) override;

private:
	void destroy() override;

	void addChildrenToTree(const std::vector<std::shared_ptr<GameObject>>* children);
	void setChildrenAsSelected(const std::vector<std::shared_ptr<GameObject>>* children);
};
