#pragma once

#include <vector>
#include <string>
#include <memory>

#include "../core/Window.h"
#include "../core/GameObject.h"
#include "Panel.h"

class EngineGUI
{
private:
	static std::vector<std::unique_ptr<Panel>> s_Panels;

public:
	static void Init();
	static void Update(const float& deltaTime);
	static void CleanUp();

private:
	static void applyStyle();
};