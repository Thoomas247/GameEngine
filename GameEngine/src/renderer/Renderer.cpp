#include "Renderer.h"

#include "glad/gl.h"

#include "../core/Log.h"

#define DRAW_LIST_RESERVE_COUNT 1000

std::vector<Mesh*> Renderer::DrawList;
Camera* Renderer::CurrentCamera;

void Renderer::Init()
{
	DrawList.reserve(DRAW_LIST_RESERVE_COUNT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.7f, 0.3f, 0.4f, 1.0f);
}

void Renderer::Draw()
{
	if (CurrentCamera == nullptr)
	{
		LOG_ERROR("RENDERER::Camera is null!")
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CurrentCamera->CalcViewProjectionMatrix();

	for (Mesh* mesh : Renderer::DrawList)
	{
		mesh->GetShader()->Activate();

		// albedo texture
		glBindTextureUnit(0, mesh->GetMaterial()->BaseColorTexture->ID);
		mesh->GetShader()->SetInt(ALBEDO_TEX, 0);	// TODO: switch to setting material all at once (make struct in shader)

		// is mesh selected?				// temp
		mesh->GetShader()->SetInt(IS_SELECTED, mesh->IsSelected());
		mesh->SetSelected(false);	// reset for next frame

		// matrices
		mesh->GetShader()->SetMat4(MODEL_MAT, mesh->GetGlobalTransform());
		mesh->GetShader()->SetMat4(VIEW_MAT, CurrentCamera->GetViewMatrix());
		mesh->GetShader()->SetMat4(PROJECTION_MAT, CurrentCamera->GetProjectionMatrix());

		mesh->GetVertexArray()->Bind();
		glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetNumIndices(), GL_UNSIGNED_INT, 0);	// we set up the EBO, so no need to pass indices
		mesh->GetVertexArray()->Unbind();
	}

	DrawList.clear();
}