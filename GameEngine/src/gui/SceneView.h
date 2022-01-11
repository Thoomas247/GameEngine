#pragma once

#include "Panel.h"

// TODO: Add camera and accept input

class SceneView : public Panel
{
private:
	unsigned int m_FrameBuffer;
	unsigned int m_Texture;

public:
	SceneView();

	void Update() override;
	

private:
	void destroy() override;
	void drawTexture();
};