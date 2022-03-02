#pragma once

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
	glm::vec4 Color;
	glm::ivec4 Joints;
	glm::vec4 Weights;

	Vertex(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& normal = glm::vec3(0.0f), const glm::vec2& texCoord = glm::vec2(0.0f),
		const glm::vec4& color = glm::vec4(1.0f), const glm::ivec4& joints = glm::ivec4(0), const glm::vec4& weights = glm::vec4(0.0f))
	{
		Position = position;
		Normal = normal;
		TexCoord = texCoord;
		Color = color;
		Joints = joints;
		Weights = weights;
	}

	static const int NUM_VERTEX_ATTRIBS = 6;

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

		// Joints
		attributeDescriptions[4].binding = 0;
		attributeDescriptions[4].location = 4;
		attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SINT;
		attributeDescriptions[4].offset = offsetof(Vertex, Joints);

		// Weights
		attributeDescriptions[5].binding = 0;
		attributeDescriptions[5].location = 5;
		attributeDescriptions[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[5].offset = offsetof(Vertex, Weights);

		return attributeDescriptions;
	}

};


const std::vector<Vertex> DEFAULT_VERTICES = { Vertex(glm::vec3(-0.5f, 0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.4f, 0.0f, 0.0f, 1.0f)),
												Vertex(glm::vec3(0.0f, 0.5f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.4f, 0.0f, 1.0f)),
												Vertex(glm::vec3(0.5f, 0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.4f, 1.0f)),
												Vertex(glm::vec3(0.0f, -0.5f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f)) };

const std::vector<uint16_t> DEFAULT_INDICES = { 0, 1, 2, 2, 3, 0 };


class MeshInfo
{
public:
	uint32_t NumIndices;

	VkBuffer VertexBuffer;
	VkDeviceMemory VertexBufferMemory;
	VkBuffer IndexBuffer;
	VkDeviceMemory IndexBufferMemory;
	VkPipelineVertexInputStateCreateInfo VertexInputCreateInfo;
	VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo RasterizationCreateInfo;

public:
	MeshInfo(const std::vector<Vertex>& vertices = DEFAULT_VERTICES, const std::vector<uint16_t>& indices = DEFAULT_INDICES);
	~MeshInfo();

	void Cleanup();
};
