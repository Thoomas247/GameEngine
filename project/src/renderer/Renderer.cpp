#include "Renderer.h"

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_CurrentCamera->CalcViewProjectionMatrix();

	for (MeshData* meshData : g_MeshDataList)
	{
		for (RenderData* renderData : meshData->RenderData)	// change to instance rendering
		{
			glUseProgram(renderData->ShaderProgram.m_GLID);

			// albedo texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderData->RenderMaterial.BaseColorTexture);
			renderData->ShaderProgram.SetInt("base_texture", 0);

			// matrices
			renderData->ShaderProgram.SetMat4("model", *renderData->Transform);
			renderData->ShaderProgram.SetMat4("view", g_CurrentCamera->m_ViewMatrix);
			renderData->ShaderProgram.SetMat4("projection", g_CurrentCamera->m_ProjectionMatrix);

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
	g_MeshDataList.erase(g_MeshDataList.begin() + index);
}
