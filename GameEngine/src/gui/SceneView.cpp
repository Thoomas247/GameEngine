#include "SceneView.h"

#include "imgui/imgui.h"
#include "glad/gl.h"

#include "../renderer/Renderer.h"
#include "../core/Log.h"

constexpr auto VIEWPORT_RENDER_RESOLUTION_WIDTH = 1920;
constexpr auto VIEWPORT_RENDER_RESOLUTION_HEIGHT = 1080;

// PUBLIC
SceneView::SceneView()
{
	glCreateFramebuffers(1, &m_FrameBuffer);

	// color texture
	glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture);
	glTextureParameteri(m_Texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(m_Texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(m_Texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_Texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(m_Texture, 1, GL_RGB8, VIEWPORT_RENDER_RESOLUTION_WIDTH, VIEWPORT_RENDER_RESOLUTION_HEIGHT);

	glNamedFramebufferTexture(m_FrameBuffer, GL_COLOR_ATTACHMENT0, m_Texture, 0);

	// depth buffer
	unsigned int depthBuffer;
	glCreateRenderbuffers(1, &depthBuffer);
	glNamedRenderbufferStorage(depthBuffer, GL_DEPTH_COMPONENT, VIEWPORT_RENDER_RESOLUTION_WIDTH, VIEWPORT_RENDER_RESOLUTION_HEIGHT);
	
	glNamedFramebufferRenderbuffer(m_FrameBuffer, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	//glNamedFramebufferDrawBuffer(m_FrameBuffer, GL_COLOR_ATTACHMENT0);

	// check for errors
	if (glCheckNamedFramebufferStatus(m_FrameBuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("SCENE_VIEW::Failed to set up rendering to ImGui!")
		return;
	}
}

void SceneView::Update()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Scene View", (bool*)0, flags);
	ImVec2 size = ImGui::GetContentRegionAvail();
	//////// temp fix: set current camera aspect ratio ////////
	Renderer::CurrentCamera->AspectRatio = size.x / size.y;
	///////////////////////////////////////////////////////////
	drawTexture();
	ImGui::Image((void*)m_Texture, size, ImVec2(0, 1), ImVec2(1, 0));	// image needs to be inverted
	ImGui::End();
}

// PRIVATE
void SceneView::destroy()
{
}

void SceneView::drawTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
	glViewport(0, 0, VIEWPORT_RENDER_RESOLUTION_WIDTH, VIEWPORT_RENDER_RESOLUTION_HEIGHT);

	Renderer::Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}