#pragma once
#include "Component.h"

#include "../../graphics/vulkan/VulkanPipeline.h"
#include "../../graphics/shaders/MaterialShader.h"


const std::vector<Vertex> DEFAULT_VERTICES = { Vertex(glm::vec3(-0.5f, 0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.4f, 0.0f, 0.0f, 1.0f)),
												Vertex(glm::vec3(0.0f, 0.5f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.4f, 0.0f, 1.0f)),
												Vertex(glm::vec3(0.5f, 0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.4f, 1.0f)),
												Vertex(glm::vec3(0.0f, -0.5f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f)) };

const std::vector<uint32_t> DEFAULT_INDICES = { 0, 1, 2, 2, 3, 0 };

constexpr auto DEFAULT_SHADER_PATH = "assets/shaders/PBRShader.glsl";


class MeshComponent : public Component
{
public:
	MeshBuffers Buffers;
	MaterialShader Shader;

public:
	MeshComponent(Entity* entity, const std::vector<Vertex>& vertices = DEFAULT_VERTICES, const std::vector<uint32_t>& indices = DEFAULT_INDICES, const std::string& shaderGlslPath = DEFAULT_SHADER_PATH);
};
