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

void Panel::Update(const float&)
{
}

// PROTECTED
void Panel::destroy()
{
}