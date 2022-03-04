#include "precompiled.h"
#include "MeshBuffers.h"


/* -- PUBLIC -- */

MeshBuffers::MeshBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	: VertexBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, sizeof(vertices[0])* vertices.size(), (void*)vertices.data()), 
	  IndexBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, sizeof(indices[0])* indices.size(), (void*)indices.data())
{
	NumIndices = indices.size();
}
