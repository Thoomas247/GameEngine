#pragma once

#include <vector>

#include "MeshData.h"
#include "Camera.h"

namespace Renderer
{
	extern std::vector<MeshData*> g_MeshDataList;
	extern Camera* g_CurrentCamera;

	void Reserve();
	void SetUp();
	void Draw();

	int AddMeshdata(MeshData* meshData);
	void RemoveMeshData(const int& index);
}