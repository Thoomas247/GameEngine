#pragma once

#include <vector>

#include "MeshData.h"
#include "Camera.h"

namespace Renderer
{
	extern std::vector<MeshData*> MeshDataList;
	extern Camera* CurrentCamera;

	void Init();
	void Draw();

	int AddMeshdata(MeshData* meshData);
	void RemoveMeshData(const int& index);
}