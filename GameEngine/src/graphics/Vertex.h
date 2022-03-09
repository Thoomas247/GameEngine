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

	Vertex(const std::vector<float>& floats)
	{
		if (floats.size() != NUM_VERTEX_FLOATS)
		{
			LOG_ERROR("VERTEX::The passed float vector is the wrong size! It is size " + std::to_string(floats.size()) + " (should be " + std::to_string(NUM_VERTEX_FLOATS) + ").");
		}

		// position
		Position.x = floats[0];
		Position.y = floats[1];
		Position.z = floats[2];

		// normal
		Normal.x = floats[3];
		Normal.y = floats[4];
		Normal.z = floats[5];

		// textCoord
		TexCoord.x = floats[6];
		TexCoord.y = floats[7];

		// color
		Color.x = floats[8];
		Color.y = floats[9];
		Color.z = floats[10];
		Color.z = floats[11];
	}

	std::vector<float> Serialize()
	{
		std::vector<float> floats;

		// position
		floats.push_back(Position.x);
		floats.push_back(Position.y);
		floats.push_back(Position.z);

		// normal
		floats.push_back(Normal.x);
		floats.push_back(Normal.y);
		floats.push_back(Normal.z);

		// textCoord
		floats.push_back(TexCoord.x);
		floats.push_back(TexCoord.y);

		// color
		floats.push_back(Color.r);
		floats.push_back(Color.g);
		floats.push_back(Color.b);
		floats.push_back(Color.a);

		return floats;
	}

	static const int NUM_VERTEX_ATTRIBS = 4;
	static const int NUM_VERTEX_FLOATS = 3 + 3 + 2 + 4;

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

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}
};