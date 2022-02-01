#pragma once

#include <vector>

#include "../structs/Vertex.h"

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
