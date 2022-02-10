#include "SceneHierarchy.h"

// PUBLIC
void SceneHierarchy::Update(const float&)
{
	ImGui::Begin("Scene Hierarchy", (bool*)0, s_WindowFlags);

	addChildrenToTree(SceneManager::GetSceneGameObjects());

	if (s_SelectedGameObject)
	{
		s_SelectedGameObject->SetSelected(true);
		setChildrenAsSelected(s_SelectedGameObject->GetChildren());
	}

	ImGui::End();
}

// PRIVATE
void SceneHierarchy::destroy()
{
}

void SceneHierarchy::addChildrenToTree(const std::vector<std::shared_ptr<GameObject>>* children)
{
	for (const auto& object : *children)
	{
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (s_SelectedGameObject == object.get())
		{
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
		}

		// open as branch if object has children
		const auto* objectChildren = object->GetChildren();

		if (objectChildren->size() == 0)
		{
			nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			ImGui::PushID((int)object->GetUUID());
			ImGui::TreeNodeEx(object->GetName().c_str(), nodeFlags);
			if (ImGui::IsItemClicked())
			{
				s_SelectedGameObject = object.get();
			}
			ImGui::PopID();
		}

		// else open as leaf
		else
		{
			nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

			ImGui::PushID((int)object->GetUUID());
			bool expanded = ImGui::TreeNodeEx(object->GetName().c_str(), nodeFlags);
			if (ImGui::IsItemClicked())
			{
				s_SelectedGameObject = object.get();
			}
			if (expanded)
			{
				addChildrenToTree(objectChildren);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}
}

void SceneHierarchy::setChildrenAsSelected(const std::vector<std::shared_ptr<GameObject>>* children)
{
	for (const auto& child : *children)
	{
		child->SetSelected(true);
		setChildrenAsSelected(child->GetChildren());
	}
}