#pragma once

//#include "graphics assets/TextureAsset.h"
//#include "graphics assets/ShaderAsset.h"
//#include "VertexArray.h"

constexpr auto DEFAULT_VERTEX_SHADER_PATH = "assets/shaders/Base.vert";
constexpr auto DEFAULT_FRAGMENT_SHADER_PATH = "assets/shaders/Base.frag";

class GraphicsAssetManager
{
private:
	static const  std::vector<Vertex> c_SquareVertices;
	static const std::vector<unsigned int> c_SquareIndices;

	static std::map<std::string, unsigned int> s_UploadedTextureIDs;

public:
	static void Init();

	//static unsigned int LoadTexture(const std::string& absolutePath = "Default");
	//static VertexArray LoadVertexArray(const std::vector<Vertex> vertices = c_SquareVertices, const std::vector<unsigned int>& indices = c_SquareIndices);

private:
};
