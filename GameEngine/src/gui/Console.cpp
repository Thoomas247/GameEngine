#include "Console.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "../core/Log.h"

/*
	Extracted from ImGui demo window
*/

int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

// PUBLIC
Console::Console()
{
	clearLog();
	memset(m_InputBuf, 0, sizeof(m_InputBuf));
	m_HistoryPos = -1;
	m_ScrollToBottom = false;
}

void Console::Update(const float& deltaTime)
{
	ImGui::Begin("Console");

	// TODO: display items starting from the bottom

	if (ImGui::SmallButton("Clear")) { clearLog(); }
	ImGui::SameLine();
	bool copy_to_clipboard = ImGui::SmallButton("Copy");

	ImGui::Separator();

	// Options, Filter
	/*
	ImGui::SameLine();
	m_Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
	ImGui::Separator();
	*/

	// Reserve enough left-over height for 1 separator + 1 input text
	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Clear")) clearLog();
		ImGui::EndPopup();
	}

	// Display every line as a separate entry so we can change their color or add custom widgets.
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	if (copy_to_clipboard)
		ImGui::LogToClipboard();

	for (int i = 0; i < Log::Items.Size; i++)
	{
		const char* item = Log::Items[i];

		//if (!m_Filter.PassFilter(item))
		//	continue;

		ImVec4 color;
		bool has_color = false;
		if (strstr(item, "[error]"))
		{
			color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);	// red
			has_color = true;
		}
		else if (strstr(item, "[warning]"))
		{
			color = ImVec4(0.9f, 1.0f, 0.4f, 1.0f);	// yellow
			has_color = true;
		}
		else if (strstr(item, "[info]"))
		{
			color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);	// grey
			has_color = true;
		}

		if (has_color)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(item);
			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::TextUnformatted(item);
		}
	}

	if (copy_to_clipboard)
		ImGui::LogFinish();

	if (m_ScrollToBottom || (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
		ImGui::SetScrollHereY(1.0f);
	m_ScrollToBottom = false;

	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();

	ImGui::End();
}

// PRIVATE
void Console::destroy()
{
	clearLog();
	for (int i = 0; i < m_History.Size; i++)
		free(m_History[i]);
}

void Console::clearLog()
{
	for (int i = 0; i < Log::Items.Size; i++)
		free(Log::Items[i]);
	Log::Items.clear();
}