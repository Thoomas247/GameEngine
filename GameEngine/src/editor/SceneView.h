#pragma once

#include "Panel.h"

#include "../renderer/Camera.h"

class SceneCamera : public Camera
{
private:
	float m_Speed = 100.0f;
	float m_MouseSensitivity = 0.1f;

public:
	void DoMovement(const float& deltaTime);
};

class SceneView : public Panel
{
private:
	bool m_IsFocused = false;
	unsigned int m_FrameBuffer;
	unsigned int m_RenderTextureID;
	SceneCamera m_SceneCamera;

public:
	SceneView();
	void Update(const float& deltaTime) override;

private:
	void destroy() override;
	void drawTexture();
};