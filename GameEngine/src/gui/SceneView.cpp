#include "SceneView.h"

#include "glad/gl.h"
#include "glm/glm.hpp"

#include "../renderer/Renderer.h"
#include "../core/Input.h"
#include "../core/Window.h"

constexpr auto VIEWPORT_RENDER_RESOLUTION_WIDTH = 1920;
constexpr auto VIEWPORT_RENDER_RESOLUTION_HEIGHT = 1080;

// PUBLIC - SceneCamera
void SceneCamera::DoMovement(const float& deltaTime)
{
	glm::vec3 front = glm::mat3_cast(LocalRotation) * glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::mat3_cast(LocalRotation) * glm::vec3(1.0f, 0.0f, 0.0f);

	// rotation
	LocalRotation = glm::rotate(LocalRotation, glm::radians(Input::GetMouseDeltaX() * m_MouseSensitivity), glm::vec3(0.0f, -1.0f, 0.0f));
	LocalRotation = glm::rotate(LocalRotation, glm::radians(Input::GetMouseDeltaY() * m_MouseSensitivity), glm::vec3(1.0f, 0.0f, 0.0f));

	// movement
	glm::vec3 inputVector = glm::vec3(0.0f);

	if (Input::GetActionMoveForward())
	{
		inputVector.z += 1.0f;
	}
	if (Input::GetActionMoveBack())
	{
		inputVector.z -= 1.0f;
	}
	if (Input::GetActionMoveRight())
	{
		inputVector.x += 1.0f;
	}
	if (Input::GetActionMoveLeft())
	{
		inputVector.x -= 1.0f;
	}

	if (inputVector != glm::vec3(0.0f))
	{
		inputVector = glm::normalize(inputVector);
		LocalPosition += front * inputVector.z * m_Speed * deltaTime;
		LocalPosition += right * inputVector.x * m_Speed * deltaTime;
	}

	Update(deltaTime);
}

// PUBLIC
SceneView::SceneView()
{
	glCreateFramebuffers(1, &m_FrameBuffer);

	// color texture
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RenderTextureID);
	glTextureParameteri(m_RenderTextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(m_RenderTextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(m_RenderTextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_RenderTextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(m_RenderTextureID, 1, GL_RGB8, VIEWPORT_RENDER_RESOLUTION_WIDTH, VIEWPORT_RENDER_RESOLUTION_HEIGHT);

	glNamedFramebufferTexture(m_FrameBuffer, GL_COLOR_ATTACHMENT0, m_RenderTextureID, 0);

	// depth buffer
	unsigned int depthBuffer;
	glCreateRenderbuffers(1, &depthBuffer);
	glNamedRenderbufferStorage(depthBuffer, GL_DEPTH_COMPONENT, VIEWPORT_RENDER_RESOLUTION_WIDTH, VIEWPORT_RENDER_RESOLUTION_HEIGHT);

	glNamedFramebufferRenderbuffer(m_FrameBuffer, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	//glNamedFramebufferDrawBuffer(m_FrameBuffer, GL_COLOR_ATTACHMENT0);

	// check for errors
	if (glCheckNamedFramebufferStatus(m_FrameBuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("SCENE_VIEW::Failed to set up rendering to ImGui!");
		return;
	}
}

void SceneView::Update(const float& deltaTime)
{
	ImGui::Begin("Scene View", (bool*)0, s_WindowFlags);
	ImVec2 size = ImGui::GetContentRegionAvail();

	if (Input::GetMouseRightButton() && ImGui::IsWindowHovered())	// if mouse is over scene view and right button clicked
	{
		m_IsFocused = true;
	}
	else if (m_IsFocused && Input::GetMouseRightButton())	// if right button is still down and scene was previously hovered over, continue focusing the scene
	{
		m_IsFocused = true;
	}
	else
	{
		m_IsFocused = false;
	}

	if (m_IsFocused)
	{
		Window::LockCursor();
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		m_SceneCamera.DoMovement(deltaTime);
	}
	else
	{
		Window::UnlockCursor();
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
	}

	m_SceneCamera.SetAspectRatio(size.x / size.y);
	Renderer::SetCurrentCamera(&m_SceneCamera);

	drawTexture();
	ImGui::Image((ImTextureID)m_RenderTextureID, size, ImVec2(0, 1), ImVec2(1, 0));	// image needs to be inverted

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