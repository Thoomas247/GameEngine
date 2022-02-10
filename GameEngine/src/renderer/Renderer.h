#pragma once

#include "../core/Scene.h"

class Renderer
{
public:
	static void Init();
	static void Draw(Scene& scene);
};