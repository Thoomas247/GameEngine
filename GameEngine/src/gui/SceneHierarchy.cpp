#include "SceneHierarchy.h"

#include "../core/World.h"

// PUBLIC
void SceneHierarchy::Update(const float& deltaTime)
{
	ImGui::Begin("Scene Hierarchy", (bool*)0, s_WindowFlags);

	addChildrenToTree(World::s_GameObjects);

	if (s_SelectedGameObject)
	{
		s_SelectedGameObject->SetSelected(true);
		setChildrenAsSelected(s_SelectedGameObject->Children);
	}

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

		if (s_SelectedGameObject == object.get())
		{
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
		}

		// open as branch if has children
		const auto& children = object->Children;

		if (children.size() == 0)
		{
			nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			ImGui::TreeNodeEx(name.c_str(), nodeFlags);
			if (ImGui::IsItemClicked())
			{
				s_SelectedGameObject = object.get();
			}
		}

		// else open as leaf
		else
		{
			nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

			bool expanded = ImGui::TreeNodeEx(name.c_str(), nodeFlags);
			if (ImGui::IsItemClicked())
			{
				s_SelectedGameObject = object.get();
			}
			if (expanded)
			{
				addChildrenToTree(children);
				ImGui::TreePop();
			}
		}
	}
}

void SceneHierarchy::setChildrenAsSelected(const std::map<std::string, std::shared_ptr<GameObject>>& children)
{
	for (auto& [name, child] : children)
	{
		child->SetSelected(true);
		setChildrenAsSelected(child->Children);
	}
}