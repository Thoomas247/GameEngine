#include "Renderer.h"

#include "glad/gl.h"

#include "../core/Log.h"
#include "../ECS/ECS.h"

// PUBLIC
void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.3f, 0.4f, 0.8f, 1.0f);
}

void Renderer::Draw()
{
	/*
	if (s_CurrentCamera == nullptr)
	{
		LOG_ERROR("RENDERER::Camera is null!");
		return;
	}
	*/

	//s_CurrentCamera->CalcViewProjectionMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (MeshComponent& meshComponent : ECS::GetMeshComponents())
	{
		// material
		Material* material = meshComponent.GetMaterial();

		glBindTextureUnit(0, material->BaseColorTexture.ID);	// albedo tex

		// shader
		Shader* shader = meshComponent.GetShader();
		shader->Activate();
		
		shader->SetInt(ALBEDO_TEX, 0);	// albedo tex

		shader->SetMat4(MODEL_MAT, meshComponent.GetModelMat());		// model
		//shader->SetMat4(VIEW_MAT, camera.GetViewMatrix());				// view
		//shader->SetMat4(PROJECTION_MAT, camera.GetProjectionMatrix());	// projection

		// vertex array
		VertexArray* vertexArray = meshComponent.GetVertexArray();
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray->GetNumIndices(), GL_UNSIGNED_INT, 0);	// we set up the EBO, so no need to pass indices
		vertexArray->Unbind();
	}
}