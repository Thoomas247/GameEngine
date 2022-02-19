#include "precompiled.h"
#include "Renderer.h"

#include "../ECS/ECS.h"
#include "../ECS/components/MeshComponent.h"
#include "../ECS/components/TransformComponent.h"

glm::mat4 Renderer::s_ViewMatrix = glm::lookAt(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.f));
glm::mat4 Renderer::s_ProjectionMatrix = glm::perspective<float>(glm::radians(60.0f), 16 / 9, 0.1f, 1000.0f);


/* -- PUBLIC -- */

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
		LOG_ERROR("RENDERER::No camera has been set!");
	}
	*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& meshComponent : ECS::GetComponents<MeshComponent>())
	{
		// shader
		MaterialShader& shader = meshComponent.GetMaterialShader();
		shader.Activate();

		shader.SetModelMat4(meshComponent.GetEntity()->GetComponent<TransformComponent>().GetGlobalTransform());	// model
		shader.SetViewMat4(s_ViewMatrix);					// view
		shader.SetProjectionMat4(s_ProjectionMatrix);		// projection

		// vertex array
		VertexArray& vertexArray = meshComponent.GetVertexArray();
		vertexArray.Bind();
		glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray.GetNumIndices(), GL_UNSIGNED_INT, 0);	// we set up the EBO, so no need to pass indices
		vertexArray.Unbind();
	}
}