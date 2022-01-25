#pragma once

#include "imgui/imgui.h"

#include "Panel.h"

class Console : public Panel
{
private:
	char m_InputBuf[256];
	ImVector<const char*> m_Commands;
	ImVector<char*> m_History;
	int m_HistoryPos;
	//ImGuiTextFilter m_Filter;
	bool m_ScrollToBottom;

public:
	Console();

	void Update(const float& deltaTime) override;

private:
	void destroy() override;
	void clearLog();
};