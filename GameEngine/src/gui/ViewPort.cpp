#include "ViewPort.h"

#include <iostream>

#include "imgui/imgui.h"
#include "glad/gl.h"

#include "../renderer/Renderer.h"

constexpr auto VIEWPORT_RENDER_RESOLUTION_WIDTH = 1920;
constexpr auto VIEWPORT_RENDER_RESOLUTION_HEIGHT = 1080;

// PUBLIC
void ViewPort::Init()
{
	glGenFramebuffers(1, &m_FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

	// The texture we're going to render to
	glGenTextures(1, &m_Texture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIEWPORT_RENDER_RESOLUTION_WIDTH, VIEWPORT_RENDER_RESOLUTION_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// The depth buffer
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, VIEWPORT_RENDER_RESOLUTION_WIDTH, VIEWPORT_RENDER_RESOLUTION_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// Set texture as our color attachment #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_Texture, 0);

	// Set the list of draw buffers.
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers); // "1" is the size of DrawBuffers

	// Unbind when done
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "RENDERER::ERROR::Failed to set up rendering to ImGui!" << std::endl;
		return;
	}
}

void ViewPort::Update()
{
	drawTexture();
	ImGui::Begin("Viewport");
	ImGui::Image((void*)m_Texture, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));	// image is inverted
	ImGui::End();
}

// PRIVATE
void ViewPort::drawTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
	glViewport(0, 0, VIEWPORT_RENDER_RESOLUTION_WIDTH, VIEWPORT_RENDER_RESOLUTION_HEIGHT);

	Renderer::Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}