#ifndef MESH_DATA
#define MESH_DATA

/*
	Contains the buffer data needed to draw meshes
	Can have many transforms for instanced rendering
*/

#include <vector>

#include "../structs/RenderData.h"

struct MeshData
{
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int NumElements;

	int IndexInRendererList;

	std::vector<RenderData*> RenderData;	// points to render data in mesh objects

	MeshData();
	~MeshData();
	MeshData(const unsigned int& vao, const unsigned int& vbo, const unsigned int& ebo, const unsigned int& numElements);
};

#endif // !MESH_DATA