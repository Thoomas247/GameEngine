#pragma once

#include "Panel.h"

#include <map>
#include <string>

class SceneHierarchy : public Panel
{
public:
	void Update(const float& deltaTime) override;

private:
	void destroy() override;

	void addChildrenToTree(const std::map<std::string, std::shared_ptr<GameObject>>& children);
	void setChildrenAsSelected(const std::map<std::string, std::shared_ptr<GameObject>>& children);
};
