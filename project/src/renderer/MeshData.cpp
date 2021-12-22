#include "MeshData.h"

#include "Renderer.h"

MeshData::MeshData()
{
}

MeshData::~MeshData()
{
	Renderer::RemoveMeshData(IndexInRendererList);
}

MeshData::MeshData(const unsigned int& vao, const unsigned int& vbo, const unsigned int& ebo, const unsigned int& numElements)
{
	VAO = vao;
	VBO = vbo;
	EBO = ebo;
	NumElements = numElements;

	IndexInRendererList = Renderer::AddMeshdata(this);

	RenderData.reserve(1000);
}