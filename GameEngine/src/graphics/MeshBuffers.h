#pragma once

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
	glm::vec4 Color;

	Vertex(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& normal = glm::vec3(0.0f), const glm::vec2& texCoord = glm::vec2(0.0f),
		const glm::vec4& color = glm::vec4(1.0f))
	{
		Position = position;
		Normal = normal;
		TexCoord = texCoord;
		Color = color;
	}

	static const int NUM_VERTEX_ATTRIBS = 4;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, NUM_VERTEX_ATTRIBS> GetAttributeDescriptions()
	{
		// float: VK_FORMAT_R32_SFLOAT
		// vec2: VK_FORMAT_R32G32_SFLOAT
		// vec3 : VK_FORMAT_R32G32B32_SFLOAT
		// vec4 : VK_FORMAT_R32G32B32A32_SFLOAT
		// uvec4: VK_FORMAT_R32G32B32A32_UINT
		// double : VK_FORMAT_R64_SFLOAT

		std::array<VkVertexInputAttributeDescription, NUM_VERTEX_ATTRIBS> attributeDescriptions{};

		// Position
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, Position);

		// Normal
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, Normal);

		// TexCoord
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, TexCoord);

		// Color
		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(Vertex, Color);

		return attributeDescriptions;
	}
};


const std::vector<Vertex> DEFAULT_VERTICES = { Vertex(glm::vec3(-0.5f, 0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.4f, 0.0f, 0.0f, 1.0f)),
												Vertex(glm::vec3(0.0f, 0.5f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.4f, 0.0f, 1.0f)),
												Vertex(glm::vec3(0.5f, 0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.4f, 1.0f)),
												Vertex(glm::vec3(0.0f, -0.5f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f)) };

const std::vector<uint32_t> DEFAULT_INDICES = { 0, 1, 2, 2, 3, 0 };


class MeshBuffers
{
public:
	uint32_t NumIndices;

	VkBuffer VertexBuffer;
	VkDeviceMemory VertexBufferMemory;
	VkBuffer IndexBuffer;
	VkDeviceMemory IndexBufferMemory;

public:
	MeshBuffers(const std::vector<Vertex>& vertices = DEFAULT_VERTICES, const std::vector<uint32_t>& indices = DEFAULT_INDICES);

	void Cleanup();

private:
	void createVertexBuffer(const std::vector<Vertex>& vertices);
	void createIndexBuffer(const std::vector<uint32_t>& indices);
};
