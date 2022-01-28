#pragma once

#include <memory>
#include <map>

#include "../structs/Texture.h"
#include "VertexArray.h"
#include "Shader.h"

enum SkyboxSides
{
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

class Skybox
{
private:
	static std::unique_ptr<VertexArray> s_VertexArray;
	static std::unique_ptr<Shader> s_Shader;
	static std::map<SkyboxSides, std::shared_ptr<Texture>> s_Textures;

public:
	static void Init();

	static VertexArray* GetVertexArray() { return s_VertexArray.get(); }

	static Texture* GetTexture(const SkyboxSides& side) { return s_Textures[side].get(); }
	static void SetTexture(const SkyboxSides& side, std::shared_ptr<Texture> texture) { s_Textures[side] = texture; }
};