#pragma once

#include <vector>

#include "glm/glm.hpp"

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
};

class VertexArray
{
private:
	unsigned int m_ID = 0;
	size_t m_NumIndices = 0;

public:
	VertexArray();
	VertexArray(const std::vector<Vertex> vertices, const std::vector<unsigned int>& indices);
	~VertexArray();

	unsigned int GetID() const { return m_ID; }
	size_t GetNumIndices() const { return m_NumIndices; }

	void Bind();
	void Unbind();
};
