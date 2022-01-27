#pragma once

#include <string>
#include <map>
#include <memory>

#include "../structs/Texture.h"

class TextureManager
{
private:
	static std::map<std::string, std::shared_ptr<Texture>> s_TextureCache;

public:
	static void Init();
	static std::shared_ptr<Texture> LoadTexture(const std::string& path);
};