#include "Renderer.h"

#include <iostream>

#include "glad/gl.h"

#include "Mesh.h"

#define MESH_DATA_LIST_RESERVE_AMOUNT 1000

std::vector<MeshData*> Renderer::MeshDataList;
Camera* Renderer::CurrentCamera;

void Renderer::Init()
{
	MeshDataList.reserve(MESH_DATA_LIST_RESERVE_AMOUNT);

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
		std::cout << "RENDERER::ERROR::Camera is null!" << std::endl;
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CurrentCamera->CalcViewProjectionMatrix();

	for (MeshData* meshData : MeshDataList)
	{
		for (RenderData* renderData : meshData->m_RenderData)	// change to instanced rendering
		{
			glUseProgram(renderData->m_Shader->m_GLID);

			// albedo texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderData->m_Material->BaseColorTexture);
			renderData->m_Shader->SetInt(renderData->m_Shader->m_AlbedoTextureLocation, GL_TEXTURE0 + 0);

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
			renderData->m_Shader->SetMat4(renderData->m_Shader->m_ModelMatLocation, *renderData->m_Transform);
			renderData->m_Shader->SetMat4(renderData->m_Shader->m_ViewMatLocation, CurrentCamera->m_ViewMatrix);
			renderData->m_Shader->SetMat4(renderData->m_Shader->m_ProjectionMatLocation, CurrentCamera->m_ProjectionMatrix);

			glBindVertexArray(meshData->m_VAO);
			glDrawElements(GL_TRIANGLES, meshData->m_NumElements, GL_UNSIGNED_INT, 0);	// we set up the EBO, so no need to pass indices

			glBindVertexArray(0);	// unbind when done
		}
	}
}

int Renderer::AddMeshdata(MeshData* meshData)
{
	int index = MeshDataList.size();
	MeshDataList.push_back(meshData);
	return index;
}

void Renderer::RemoveMeshData(const int& index)
{
	if (MeshDataList.size() == 0)
		return;

	MeshDataList[index] = MeshDataList[MeshDataList.size() - 1];
	MeshDataList[index]->m_IndexInRendererList = index;
	MeshDataList.erase(MeshDataList.end() - 1);
}