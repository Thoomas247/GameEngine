#include "MeshData.h"

#include "Renderer.h"

MeshData::MeshData()
{
}

MeshData::~MeshData()
{
	Renderer::RemoveMeshData(m_IndexInRendererList);
}

MeshData::MeshData(const unsigned int& vao, const unsigned int& numElements)
{
	m_VAO = vao;
	m_NumElements = numElements;
	m_IndexInRendererList = Renderer::AddMeshdata(this);

	m_RenderData.reserve(1000);
}