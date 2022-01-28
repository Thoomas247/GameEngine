#include "Panel.h"

GameObject* Panel::s_SelectedGameObject = nullptr;

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