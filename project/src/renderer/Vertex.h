#ifndef VERTEX
#define VERTEX

#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
	glm::vec4 Color;
	glm::ivec4 Joints;
	glm::vec4 Weights;

	Vertex()
	{
		Position = glm::vec3(0.0f);
		Normal = glm::vec3(0.0f);
		TexCoord = glm::vec2(0.0f);
		Color = glm::vec4(1.0f);
		Joints = glm::ivec4(0);
		Weights = glm::vec4(0.0f);
	}

	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoord)
	{
		Position = position;
		Normal = normal;
		TexCoord = texCoord;
		Color = glm::vec4(1.0f);
		Joints = glm::ivec4(0);
		Weights = glm::vec4(0.0f);
	}

	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoord, const glm::vec4& color, const glm::ivec4& joints, const glm::vec4& weights)
	{
		Position = position;
		Normal = normal;
		TexCoord = texCoord;
		Color = color;
		Joints = joints;
		Weights = weights;
	}
};

#endif // !VERTEX
