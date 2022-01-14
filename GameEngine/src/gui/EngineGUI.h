#pragma once

#include <vector>
#include <string>
#include <memory>

#include "../core/Window.h"
#include "../core/GameObject.h"
#include "Panel.h"

namespace EngineGUI
{
	extern std::vector<std::unique_ptr<Panel>> Panels;

	void Init();
	void Draw();
	void CleanUp();
};