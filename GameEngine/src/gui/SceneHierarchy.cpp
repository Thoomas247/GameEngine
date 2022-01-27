#include "SceneHierarchy.h"

#include "../core/World.h"

// PUBLIC
void SceneHierarchy::Update(const float& deltaTime)
{
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Scene Hierarchy", (bool*)0, windowFlags);

	addChildrenToTree(World::GetGameObjects());

	if (GetSelectedGameObject())
	{
		GetSelectedGameObject()->SetSelected(true);
		setChildrenAsSelected(GetSelectedGameObject()->GetChildren());
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

		if (GetSelectedGameObject() == object.get())
		{
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
		}

		// open as branch if has children
		if (object->GetChildren().size() == 0)
		{
			nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			ImGui::TreeNodeEx(name.c_str(), nodeFlags);
			if (ImGui::IsItemClicked())
			{
				SetSelectedGameObject(object.get());
			}
		}

		// else open as leaf
		else
		{
			nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

			bool expanded = ImGui::TreeNodeEx(name.c_str(), nodeFlags);
			if (ImGui::IsItemClicked())
			{
				SetSelectedGameObject(object.get());
			}
			if (expanded)
			{
				addChildrenToTree(object->GetChildren());
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
		setChildrenAsSelected(child->GetChildren());
	}
}
