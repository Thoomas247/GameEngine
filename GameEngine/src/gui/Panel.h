#pragma once

#include "imgui/imgui.h"

#include "../core/GameObject.h"
#include "../core/Log.h"

class Panel
{
private:
	static GameObject* s_SelectedGameObject;

public:
	Panel();
	~Panel();

	virtual void Update(const float& deltaTime);

	GameObject* GetSelectedGameObject() const { return s_SelectedGameObject; }
	void SetSelectedGameObject(GameObject* gameObject) { s_SelectedGameObject = gameObject; }

protected:
	virtual void destroy();
};