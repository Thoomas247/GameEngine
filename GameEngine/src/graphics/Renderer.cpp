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

	auto* meshVector = ECS::GetMeshComponents();
	for (int i = 0; i < meshVector->size(); i++)
	{
		MeshComponent& meshComponent = meshVector->at(i);

		// material
		Material* material = meshComponent.GetMaterial();

		glBindTextureUnit(0, material->BaseColorTexture.GetGLID());	// albedo tex

		// shader
		ShaderAsset* shader = meshComponent.GetShader();
		shader->Activate();

		shader->SetInt("albedo_tex", 0);	// albedo tex

		shader->SetMat4("model_mat", meshComponent.GetModelMat());	// model
		shader->SetMat4("view_mat", glm::mat4(1.0f));				// view
		shader->SetMat4("projection_mat", glm::mat4(1.0f));			// projection

		// vertex array
		VertexArrayAsset* vertexArray = meshComponent.GetVertexArray();
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray->GetNumIndices(), GL_UNSIGNED_INT, 0);	// we set up the EBO, so no need to pass indices
		vertexArray->Unbind();
	}
}