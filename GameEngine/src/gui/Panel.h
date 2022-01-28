#pragma once

#include "imgui/imgui.h"

#include "../core/GameObject.h"
#include "../core/Log.h"

class Panel
{
protected:
	static GameObject* s_SelectedGameObject;
	static ImGuiWindowFlags s_WindowFlags;

public:
	Panel();
	~Panel();

	virtual void Update(const float& deltaTime);

protected:
	virtual void destroy();
};