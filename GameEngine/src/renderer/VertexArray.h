#pragma once

#include <vector>

#include "../structs/Vertex.h"

class VertexArray
{
public:
	unsigned int ID = 0;
	unsigned int NumIndices = 0;

public:
	VertexArray(const std::vector<Vertex> vertices, const std::vector<unsigned int>& indices);
	~VertexArray();

	void Bind();
	void Unbind();
};

