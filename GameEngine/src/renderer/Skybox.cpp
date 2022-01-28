#include "Skybox.h"

#include <vector>

#include "../managers/TextureManager.h"

std::unique_ptr<VertexArray> Skybox::s_VertexArray;
std::unique_ptr<Shader> Skybox::s_Shader;
std::map<SkyboxSides, std::shared_ptr<Texture>> Skybox::s_Textures;

void Skybox::Init()
{
	std::vector<Vertex> vertices = {
		Vertex({-1.0f, -1.0f, 1.0f}),
		Vertex({1.0f, -1.0f, 1.0f}),
		Vertex({1.0f, -1.0f, -1.0f}),
		Vertex({-1.0f, -1.0f, -1.0f}),
		Vertex({-1.0f, 1.0f, 1.0f}),
		Vertex({1.0f, 1.0f, 1.0f}),
		Vertex({1.0f, 1.0f, -1.0f}),
		Vertex({-1.0f, 1.0f, -1.0f})
	};

	std::vector<unsigned int> indices = {
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};

	s_VertexArray = std::make_unique<VertexArray>(vertices, indices);

	s_Shader = std::make_unique<Shader>("assets/shaders/Skybox.vert", "assets/shaders/Skybox.frag");

	s_Textures[FRONT] = TextureManager::LoadTexture("assets/textures/skybox/skybox_front.jpg");
	s_Textures[BACK] = TextureManager::LoadTexture("assets/textures/skybox/skybox_back.jpg");
	s_Textures[LEFT] = TextureManager::LoadTexture("assets/textures/skybox/skybox_left.jpg");
	s_Textures[RIGHT] = TextureManager::LoadTexture("assets/textures/skybox/skybox_right.jpg");
	s_Textures[TOP] = TextureManager::LoadTexture("assets/textures/skybox/skybox_top.jpg");
	s_Textures[BOTTOM] = TextureManager::LoadTexture("assets/textures/skybox/skybox_bottom.jpg");
}