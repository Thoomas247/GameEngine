#include "PropertiesPanel.h"

void TextCentered(std::string text) {
	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text.c_str());
}

// PUBLIC
void PropertiesPanel::Update(const float& deltaTime)
{
	ImGui::Begin("Properties", (bool*)0, s_WindowFlags);

	if (GetSelectedGameObject())
	{
		// LOCAL TRANSFORM
		ImGui::Separator();
		TextCentered("Local Tranform");
		ImGui::Separator();

		ImGui::BeginTable("Transform Table", 4);

		// position
		glm::vec3 localPos = GetSelectedGameObject()->GetLocalPosition();

		ImGui::TableNextColumn();
		ImGui::Text("Position");

		bool posChanged = false;

		ImGui::TableNextColumn();
		ImGui::Text("X:"); ImGui::SameLine();

		ImGui::PushID("XPos");
		if (ImGui::InputFloat("", & localPos.x))
			posChanged = true;
		ImGui::PopID();

		ImGui::TableNextColumn();
		ImGui::Text("Y:"); ImGui::SameLine();

		ImGui::PushID("YPos");
		if (ImGui::InputFloat("", &localPos.y))
			posChanged = true;
		ImGui::PopID();

		ImGui::TableNextColumn();
		ImGui::Text("Z:"); ImGui::SameLine();

		ImGui::PushID("ZPos");
		if (ImGui::InputFloat("", &localPos.z))
			posChanged = true;
		ImGui::PopID();

		if (posChanged)
		{
			GetSelectedGameObject()->SetLocalPosition(localPos);
		}

		// rotation
		glm::vec3 localRot = glm::degrees(GetSelectedGameObject()->GetLocalRotationAsEulerAngles());

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
			GetSelectedGameObject()->SetLocalRotationFromEulerAngles(glm::radians(localRot));
		}

		// scale
		glm::vec3 localScale = GetSelectedGameObject()->GetLocalScale();

		ImGui::TableNextColumn();
		ImGui::Text("Scale");

		bool scaleChanged = false;

		ImGui::TableNextColumn();
		ImGui::Text("X:"); ImGui::SameLine();

		ImGui::PushID("XScale");
		if (ImGui::InputFloat("", &localScale.x))
			scaleChanged = true;
		ImGui::PopID();

		ImGui::TableNextColumn();
		ImGui::Text("Y:"); ImGui::SameLine();

		ImGui::PushID("YScale");
		if (ImGui::InputFloat("", &localScale.y))
			scaleChanged = true;
		ImGui::PopID();

		ImGui::TableNextColumn();
		ImGui::Text("Z:"); ImGui::SameLine();

		ImGui::PushID("ZScale");
		if (ImGui::InputFloat("", &localScale.z))
			scaleChanged = true;
		ImGui::PopID();

		if (scaleChanged)
		{
			GetSelectedGameObject()->SetLocalScale(localScale);
		}

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
