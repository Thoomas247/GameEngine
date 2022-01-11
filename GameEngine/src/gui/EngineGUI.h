#pragma once

#include <map>
#include <string>
#include <memory>

#include "../core/Window.h"
#include "Panel.h"

namespace EngineGUI
{
	extern std::map<std::string, std::unique_ptr<Panel>> Panels;

	void Init();
	void Draw();
	void CleanUp();
};