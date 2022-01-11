#pragma once

#include "Panel.h"

// TODO: Add camera and accept input

class ViewPort : public Panel
{
private:
	unsigned int m_FrameBuffer;
	unsigned int m_Texture;
	unsigned int m_Width;
	unsigned int m_Height;

public:
	void Init() override;
	void Update() override;

private:
	void drawTexture();
};