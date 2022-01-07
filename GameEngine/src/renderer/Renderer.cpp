#include "Renderer.h"

#include <iostream>

#include "glad/gl.h"

#include "Mesh.h"

#define MESH_DATA_LIST_RESERVE_AMOUNT 1000

std::vector<MeshData*> Renderer::g_MeshDataList;
Camera* Renderer::g_CurrentCamera;

void Renderer::Reserve()
{
	g_MeshDataList.reserve(MESH_DATA_LIST_RESERVE_AMOUNT);
}

void Renderer::SetUp()
{
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
	if (g_CurrentCamera == nullptr)
	{
		std::cout << "RENDERER::ERROR::Camera is null!" << std::endl;
		return;
	}


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_CurrentCamera->CalcViewProjectionMatrix();

	for (MeshData* meshData : g_MeshDataList)
	{
		for (RenderData* renderData : meshData->RenderData)	// change to instanced rendering
		{
			glUseProgram(renderData->ShaderProgram->m_GLID);

			// albedo texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderData->RenderMaterial->BaseColorTexture);
			renderData->ShaderProgram->SetInt(renderData->ShaderProgram->m_AlbedoTextureLocation, GL_TEXTURE0 + 0);

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

			// matrices
			renderData->ShaderProgram->SetMat4(renderData->ShaderProgram->m_ModelMatLocation, *renderData->Transform);
			renderData->ShaderProgram->SetMat4(renderData->ShaderProgram->m_ViewMatLocation, g_CurrentCamera->m_ViewMatrix);
			renderData->ShaderProgram->SetMat4(renderData->ShaderProgram->m_ProjectionMatLocation, g_CurrentCamera->m_ProjectionMatrix);

			glBindVertexArray(meshData->VAO);
			glDrawElements(GL_TRIANGLES, meshData->NumElements, GL_UNSIGNED_INT, 0);	// we set up the EBO, so no need to pass indices

			glBindVertexArray(0);	// unbind when done
		}
	}
}

int Renderer::AddMeshdata(MeshData* meshData)
{
	int index = g_MeshDataList.size();
	g_MeshDataList.push_back(meshData);
	return index;
}

void Renderer::RemoveMeshData(const int& index)
{
	if (g_MeshDataList.size() == 0)
		return;

	g_MeshDataList[index] = g_MeshDataList[g_MeshDataList.size() - 1];
	g_MeshDataList[index]->IndexInRendererList = index;
	g_MeshDataList.erase(g_MeshDataList.end() - 1);
}