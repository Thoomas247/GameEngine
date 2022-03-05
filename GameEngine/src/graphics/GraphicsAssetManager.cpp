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
