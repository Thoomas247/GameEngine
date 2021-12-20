#include "Renderer.h"

#include "glad/gl.h"

#include "RenderData.h"
#include "Mesh.h"
#include "Camera.h"

void Renderer::SetUp()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.8f, 0.3f, 0.7f, 1.0f);
}

void Renderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera* camera = (Camera*)RenderData::CurrentCamera;
	camera->CalcViewProjectionMatrix();

	for (auto& object : RenderData::Queue)
	{
		Mesh* mesh = (Mesh*)object;
		
		glUseProgram(mesh->m_ShaderProgram.m_GLID);

		// albedo texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->m_Material.BaseColorTexture);
		mesh->m_ShaderProgram.SetInt("base_texture", 0);

		// matrices
		mesh->m_ShaderProgram.SetMat4("model", mesh->m_GlobalTransform);
		mesh->m_ShaderProgram.SetMat4("view_projection", camera->m_ViewProjectionMatrix);

		glBindVertexArray(mesh->m_VAO);
		glDrawElements(GL_TRIANGLES, mesh->m_NumElements, GL_UNSIGNED_INT, 0);	// we set up the EBO, so no need to pass indices
		glBindVertexArray(0);	// unbind when done
	}

	RenderData::Queue.clear();
}
