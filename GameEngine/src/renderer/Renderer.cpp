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

	glClearColor(0.2f, 0.3f, 0.7f, 1.0f);
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
		glUseProgram(mesh->m_Shader->m_GLID);

		// albedo texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->m_Material->BaseColorTexture);
		mesh->m_Shader->SetInt(mesh->m_Shader->m_AlbedoTextureLocation, GL_TEXTURE0 + 0);

		/*
		// emissive texture
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, renderData->RenderMaterial->EmissiveTexture);
		renderData->ShaderProgram->SetInt(renderData->ShaderProgram->m_EmissiveTextureLocation, GL_TEXTURE0 + 1);

		// metallic roughness texture
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, renderData->RenderMaterial->MetallicRoughnessTexture);
		renderData->ShaderProgram->SetInt(renderData->ShaderProgram->m_MetallicRoughnessTextureLocation, GL_TEXTURE0 + 2);

		// normal texture
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, renderData->RenderMaterial->Normaltexture);
		renderData->ShaderProgram->SetInt(renderData->ShaderProgram->m_NormalTextureLocation, GL_TEXTURE0 + 3);

		// emissive texture
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, renderData->RenderMaterial->OcclusionTexture);
		renderData->ShaderProgram->SetInt(renderData->ShaderProgram->m_OcclusionTextureLocation, GL_TEXTURE0 + 4);
		*/

		// is selected?				// temp
		if (mesh->m_isSelected)
		{
			mesh->m_Shader->SetInt(mesh->m_Shader->m_IsSelectedLocation, 1);
		}
		else
		{
			mesh->m_Shader->SetInt(mesh->m_Shader->m_IsSelectedLocation, 0);
		}
		mesh->m_isSelected = false;

		// matrices
		mesh->m_Shader->SetMat4(mesh->m_Shader->m_ModelMatLocation, mesh->m_GlobalTransform);
		mesh->m_Shader->SetMat4(mesh->m_Shader->m_ViewMatLocation, CurrentCamera->ViewMatrix);
		mesh->m_Shader->SetMat4(mesh->m_Shader->m_ProjectionMatLocation, CurrentCamera->ProjectionMatrix);

		glBindVertexArray(mesh->m_BufferData->VAO);
		glDrawElements(GL_TRIANGLES, mesh->m_BufferData->NumElements, GL_UNSIGNED_INT, 0);	// we set up the EBO, so no need to pass indices

		glBindVertexArray(0);	// unbind when done
	}

	DrawList.clear();
}