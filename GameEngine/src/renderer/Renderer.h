#pragma once

#include <vector>

#include "Mesh.h"
#include "Camera.h"

namespace Renderer
{
	extern std::vector<Mesh*> DrawList;
	extern Camera* CurrentCamera;

	void Init();
	void Draw();
}