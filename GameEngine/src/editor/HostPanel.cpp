#include "HostPanel.h"

// PUBLIC
void HostPanel::Update(const float&)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, s_HostWindowFlags);
	ImGui::PopStyleVar(3);

	ImGuiID dockSpaceID = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), s_DockSpaceFlags, nullptr);

	// menu bar
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		ImGui::MenuItem("New Project", "CTRL+N");
		ImGui::MenuItem("Open Project", "CTRL+O");
		ImGui::MenuItem("Save", "CTRL+S");

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View"))
	{
		ImGui::MenuItem("Scene View");
		ImGui::MenuItem("Scene Hierarchy");
		ImGui::MenuItem("GameObject Properties");

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Add"))
	{
		ImGui::MenuItem("Empty GameObject");
		if (ImGui::BeginMenu("Mesh"))
		{
			ImGui::MenuItem("Cube");
			ImGui::MenuItem("Sphere");
			ImGui::MenuItem("Capsule");

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Collider"))
		{
			ImGui::MenuItem("Cube");
			ImGui::MenuItem("Sphere");
			ImGui::MenuItem("Capsule");
			ImGui::MenuItem("Mesh");

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	ImGui::Spacing();

	ImGui::End();
}

// PRIVATE
void HostPanel::destroy()
{
}