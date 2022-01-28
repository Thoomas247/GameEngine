#pragma once

#include "Panel.h"

class PropertiesPanel : public Panel
{
public:
	void Update(const float& deltaTime) override;

private:
	void destroy() override;
};
