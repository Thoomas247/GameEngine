#pragma once

/*
	Contains the buffer data needed to draw meshes
	Can have many transforms for instanced rendering
*/

#include <vector>

#include "RenderData.h"

class MeshData
{
public:
	unsigned int m_VAO;
	unsigned int m_NumElements;
	int m_IndexInRendererList;
	std::vector<RenderData*> m_RenderData;	// points to render data in mesh objects

public:
	MeshData();
	~MeshData();
	MeshData(const unsigned int& vao, const unsigned int& numElements);
};