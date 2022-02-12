#pragma once

#include <map>
#include <memory>

#include "graphics assets/TextureAsset.h"
#include "graphics assets/ShaderAsset.h"

constexpr auto DEFAULT_VERTEX_SHADER_PATH = "assets/shaders/Base.vert";
constexpr auto DEFAULT_FRAGMENT_SHADER_PATH = "assets/shaders/Base.frag";

class GraphicsAssetManager
{
private:
	static std::map<std::string, std::shared_ptr<TextureAsset>> s_LoadedTextures;
	static std::map<std::string, std::shared_ptr<ShaderAsset>> s_LoadedShaders;

public:
	static void Init();

	static TextureAsset LoadTexture(const std::string& absolutePath = "Default");
	static ShaderAsset LoadShader(const std::string& vertexPath = DEFAULT_VERTEX_SHADER_PATH, const std::string& fragmentPath = DEFAULT_FRAGMENT_SHADER_PATH);

private:
	static void shaderCheckCompileErrors(const unsigned int& shader, const std::string& type);
	static std::string shaderLoadFileContents(const std::string& absolutePath);
};

