#ifndef ASSET_MANAGER
#define ASSET_MANAGER

#include <memory>
#include <string>

#include "GameObject.h"

namespace AssetManager
{
	std::shared_ptr<GameObject> LoadModel(const std::string& name);
}

#endif // !ASSET_MANAGER