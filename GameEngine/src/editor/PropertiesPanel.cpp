#include "PropertiesPanel.h"

void TextCentered(std::string text) {
	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text.c_str());
}

// PUBLIC
void PropertiesPanel::Update(const float&)
{
	ImGui::Begin("Properties", (bool*)0, s_WindowFlags);

	if (s_SelectedGameObject)
	{
		TextCentered("GameObject ID: " + std::to_string(s_SelectedGameObject->GetUUID()));
		ImGui::Separator();

		// LOCAL TRANSFORM
		ImGui::Separator();
		TextCentered("Local Tranform");
		ImGui::Separator();

		bool tableReturn = ImGui::BeginTable("Transform Table", 4);

		// position
		ImGui::TableNextColumn();
		ImGui::Text("Position");

		ImGui::TableNextColumn();
		ImGui::Text("X:"); ImGui::SameLine();

		ImGui::PushID("XPos");
		ImGui::InputFloat("", &s_SelectedGameObject->LocalPosition.x);
		ImGui::PopID();

		ImGui::TableNextColumn();
		ImGui::Text("Y:"); ImGui::SameLine();

		ImGui::PushID("YPos");
		ImGui::InputFloat("", &s_SelectedGameObject->LocalPosition.y);
		ImGui::PopID();

		ImGui::TableNextColumn();
		ImGui::Text("Z:"); ImGui::SameLine();

		ImGui::PushID("ZPos");
		ImGui::InputFloat("", &s_SelectedGameObject->LocalPosition.z);
		ImGui::PopID();

		// rotation
		glm::vec3 localRot = s_SelectedGameObject->GetLocalRotationAsEulerAngles();

		ImGui::TableNextColumn();
		ImGui::Text("Rotation");

		bool rotChanged = false;

		ImGui::TableNextColumn();
		ImGui::Text("X:"); ImGui::SameLine();

		ImGui::PushID("XRot");
		if (ImGui::InputFloat("", &localRot.x))
			rotChanged = true;
		ImGui::PopID();

		ImGui::TableNextColumn();
		ImGui::Text("Y:"); ImGui::SameLine();

		ImGui::PushID("YRot");
		if (ImGui::InputFloat("", &localRot.y))
			rotChanged = true;
		ImGui::PopID();

		ImGui::TableNextColumn();
		ImGui::Text("Z:"); ImGui::SameLine();

		ImGui::PushID("ZRot");
		if (ImGui::InputFloat("", &localRot.z))
			rotChanged = true;
		ImGui::PopID();

		if (rotChanged)
		{
			s_SelectedGameObject->SetLocalRotationFromEulerAngles(localRot);
		}

		// scale
		ImGui::TableNextColumn();
		ImGui::Text("Scale");

		ImGui::TableNextColumn();
		ImGui::Text("X:"); ImGui::SameLine();

		ImGui::PushID("XScale");
		ImGui::InputFloat("", &s_SelectedGameObject->LocalScale.x);
		ImGui::PopID();

		ImGui::TableNextColumn();
		ImGui::Text("Y:"); ImGui::SameLine();

		ImGui::PushID("YScale");
		ImGui::InputFloat("", &s_SelectedGameObject->LocalScale.y);
		ImGui::PopID();

		ImGui::TableNextColumn();
		ImGui::Text("Z:"); ImGui::SameLine();

		ImGui::PushID("ZScale");
		ImGui::InputFloat("", &s_SelectedGameObject->LocalScale.z);
		ImGui::PopID();

		if (tableReturn)
			ImGui::EndTable();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		// OTHER
		TextCentered("Other Stuff");
		ImGui::Separator();

		ImGui::Text("More Stuff");
		ImGui::Text("Even More Stuff");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
	}

	ImGui::End();
}

// PRIVATE
void PropertiesPanel::destroy()
{
}