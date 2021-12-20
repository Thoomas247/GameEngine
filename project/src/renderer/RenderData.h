#ifndef RENDER_QUEUE
#define RENDER_QUEUE

#include <memory>
#include <vector>

#include "../core/GameObject.h"

namespace RenderData
{
	extern std::vector<GameObject*> Queue;
	extern GameObject* CurrentCamera;
}

#endif // !RENDER_QUEUE