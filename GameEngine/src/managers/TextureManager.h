#pragma once

#include <string>
#include <map>
#include <memory>

#include "../structs/Texture.h"

namespace TextureManager
{
	extern std::map<std::string, std::shared_ptr<Texture>> TextureCache;

	void Init();
	std::shared_ptr<Texture> LoadTexture(const std::string& path);
}