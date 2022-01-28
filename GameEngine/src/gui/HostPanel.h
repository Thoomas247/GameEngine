#pragma once

#include "Panel.h"

#include "imgui/imgui_internal.h"

class HostPanel : public Panel
{
private:
	static const ImGuiDockNodeFlags s_DockSpaceFlags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
	static const ImGuiWindowFlags s_HostWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

public:
	void Update(const float& deltaTime) override;

private:
	void destroy() override;
};
