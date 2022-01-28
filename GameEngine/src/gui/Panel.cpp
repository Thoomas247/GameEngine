#include "Panel.h"

GameObject* Panel::s_SelectedGameObject = nullptr;
ImGuiWindowFlags Panel::s_WindowFlags = ImGuiWindowFlags_NoCollapse;

// PUBLIC
Panel::Panel()
{

}

Panel::~Panel()
{
	destroy();
}

void Panel::Update(const float& deltaTime)
{
}

// PROTECTED
void Panel::destroy()
{
}
