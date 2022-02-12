#pragma once

#include <map>
#include <memory>

#include "graphics assets/TextureAsset.h"
#include "graphics assets/ShaderAsset.h"
#include "graphics assets/VertexArrayAsset.h"

constexpr auto DEFAULT_VERTEX_SHADER_PATH = "assets/shaders/Base.vert";
constexpr auto DEFAULT_FRAGMENT_SHADER_PATH = "assets/shaders/Base.frag";

class GraphicsAssetManager
{
private:
	static const  std::vector<Vertex> c_SquareVertices;
	static const std::vector<unsigned int> c_SquareIndices;

	static std::map<std::string, std::shared_ptr<TextureAsset>> s_UploadedTextures;
	static std::map<std::string, std::shared_ptr<ShaderAsset>> s_UploadedShaders;

public:
	static void Init();

	static TextureAsset LoadTexture(const std::string& absolutePath = "Default");
	static ShaderAsset LoadShader(const std::string& vertexPath = DEFAULT_VERTEX_SHADER_PATH, const std::string& fragmentPath = DEFAULT_FRAGMENT_SHADER_PATH);
	static VertexArrayAsset LoadVertexArray(const std::vector<Vertex> vertices = c_SquareVertices, const std::vector<unsigned int>& indices = c_SquareIndices);

private:
	static void shaderCheckCompileErrors(const unsigned int& shader, const std::string& type);
	static std::string shaderLoadFileContents(const std::string& absolutePath);
};

