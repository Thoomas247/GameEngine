#include "precompiled.h"
#include "GraphicsAssetManager.h"

const std::vector<Vertex> GraphicsAssetManager::c_SquareVertices = { Vertex(glm::vec3(0.0f, 0.0f, -1.0f)), Vertex(glm::vec3(0.0f, 1.0f, -1.0f)), Vertex(glm::vec3(1.0f, 0.0f, 1.0f)), Vertex(glm::vec3(1.0f, 1.0f, 1.0f)) };
const std::vector<unsigned int> GraphicsAssetManager::c_SquareIndices = { 0, 2, 1, 1, 2, 3 };

std::map<std::string, unsigned int> GraphicsAssetManager::s_UploadedTextureIDs;


/* -- PUBLIC -- */

/// <summary>
/// Prepares all of the default textures and shaders.
/// Required for proper rendering.
/// </summary>
void GraphicsAssetManager::Init()
{
	
}

unsigned int GraphicsAssetManager::LoadTexture(const std::string& absolutePath)
{
	return 0;
}

VertexArray GraphicsAssetManager::LoadVertexArray(const std::vector<Vertex> vertices, const std::vector<unsigned int>& indices)
{
	return VertexArray();
}


/* -- PRIVATE -- */

void GraphicsAssetManager::shaderCheckCompileErrors(const unsigned int& shader, const std::string& type)
{
	
}

std::string GraphicsAssetManager::shaderLoadFileContents(const std::string& absolutePath)
{
	std::string fileContents;
	std::ifstream fileStream;

	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		fileStream.open(absolutePath);

		std::stringstream stringStream;
		stringStream << fileStream.rdbuf();

		fileStream.close();

		fileContents = stringStream.str();
	}
	catch (std::ifstream::failure&) {
		LOG_ERROR("SHADER::File at path " + absolutePath + " not successfully read. If trying to load the default texture, make sure GraphicsAssetManager has been initialized.");
	}

	return fileContents;
}