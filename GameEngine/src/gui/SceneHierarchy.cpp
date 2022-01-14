#include "SceneHierarchy.h"

#include "imgui/imgui.h"

#include "../core/World.h"

// PUBLIC
void SceneHierarchy::Update()
{
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Scene Hierarchy", (bool*)0, windowFlags);

	addChildrenToTree(World::GameObjects);
	if (Selected)	// temp
		Selected->m_isSelected = true;

	ImGui::End();
}

// PRIVATE
void SceneHierarchy::destroy()
{

}

void SceneHierarchy::addChildrenToTree(const std::map<std::string, std::shared_ptr<GameObject>>& children)
{
	for (const auto& [name, object] : children)
	{
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (Selected == object)
		{
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
		}

		if (object->m_Children.size() == 0)
		{
			nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			ImGui::TreeNodeEx(name.c_str(), nodeFlags);
			if (ImGui::IsItemClicked())
			{
				Selected = object;
			}
		}

		else
		{
			nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

			bool expanded = ImGui::TreeNodeEx(name.c_str(), nodeFlags);
			if (ImGui::IsItemClicked())
			{
				Selected = object;
			}
			if (expanded)
			{
				addChildrenToTree(object->m_Children);
				ImGui::TreePop();
			}
		}
	}
}
