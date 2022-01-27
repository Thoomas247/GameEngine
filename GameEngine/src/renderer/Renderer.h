#pragma once

#include <vector>

#include "Mesh.h"
#include "Camera.h"

class Renderer
{
private:
	static std::vector<Mesh*> s_DrawList;
	static Camera* s_CurrentCamera;

public:
	static void Init();
	static void Draw();

	static void AddToDrawList(Mesh* mesh) { s_DrawList.push_back(mesh); }
	static void SetCurrentCamera(Camera* camera) { s_CurrentCamera = camera; }
};