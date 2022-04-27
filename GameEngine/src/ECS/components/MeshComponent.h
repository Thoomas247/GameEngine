#pragma once
#include "Component.h"

#include "../../vulkan/VulkanBuffer.h"
#include "../../vulkan/VulkanPipeline.h"


const std::vector<Vertex> DEFAULT_VERTICES = { Vertex(glm::vec3(-0.5f, 0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.4f, 0.0f, 0.0f, 1.0f)),
												Vertex(glm::vec3(0.0f, 0.5f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.4f, 0.0f, 1.0f)),
												Vertex(glm::vec3(0.5f, 0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.4f, 1.0f)),
												Vertex(glm::vec3(0.0f, -0.5f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) };

const std::vector<uint32_t> DEFAULT_INDICES = { 0, 1, 2, 2, 3, 0 };

constexpr auto DEFAULT_SHADER_PATH = "assets/shaders/PBRShader.glsl";
//constexpr auto DEFAULT_SHADER_PATH = "C:/Users/TM1/source/repos/GameEngine/GameEngine/assets/shaders/PBRShader.glsl";


class MeshComponent : public Component
{
public:
	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	uint32_t NumIndices;

	// TODO: Make Mesh class to store the 3 member vars above

	MaterialShader Shader;

private:
	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;
	std::string m_GlslPath;

public:
	MeshComponent(Entity* entity, const std::vector<Vertex>& vertices = DEFAULT_VERTICES, const std::vector<uint32_t>& indices = DEFAULT_INDICES, const std::string& shaderGlslPath = DEFAULT_SHADER_PATH);
	MeshComponent(Entity* entity, json j);

	json Serialize();
};
