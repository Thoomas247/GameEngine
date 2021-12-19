#include "AssetManager.h"

#include <map>
#include <fstream>

#include "glm/glm.hpp"
#include "json/json.hpp"
using json = nlohmann::json;

#include <iostream>

#include "ProjectManager.h"
#include "../renderer/Mesh.h"

std::shared_ptr<GameObject> AssetManager::LoadModel(const std::string& name)
{
	std::string path = ProjectManager::ProjectPath + ProjectManager::DefaultModelPath + name;

	std::streamsize size = std::filesystem::file_size(path);
	std::vector<char> buffer;
	buffer.resize(size);


	std::ifstream fileIn = std::ifstream(path, std::ios::in | std::ios::binary);
	fileIn.read(&buffer[0], size);
	fileIn.close();

	json j = json::from_bson(buffer);

	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();

	for (auto& [name, jmesh] : j["meshes"].items())
	{
		std::vector<float> positions = jmesh["positions"];
		std::vector<float> normals = jmesh["normals"];
		std::vector<float> texCoords = jmesh["texCoords"];
		std::vector<float> colors = jmesh["colors"];
		std::vector<int> joints = jmesh["joints"];
		std::vector<float> weights = jmesh["weights"];

		std::vector<Vertex> vertices;
		int arraySize = (int)positions.size() / 3; // 3 is num of components in a position vec3
		vertices.reserve(arraySize);

		for (int i = 0; i < arraySize; i++)
		{
			Vertex v = Vertex(glm::vec3(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]),
							glm::vec3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]),
							glm::vec2(texCoords[i * 2 + 0], texCoords[i * 2 + 1]),
							glm::vec4(colors[i * 4 + 0], colors[i * 4 + 1], colors[i * 4 + 2], colors[i * 4 + 3]),
							glm::ivec4(joints[i * 4 + 0], joints[i * 4 + 1], joints[i * 4 + 2], joints[i * 4 + 3]),
							glm::vec4(weights[i * 4 + 0], weights[i * 4 + 1], weights[i * 4 + 2], weights[i * 4 + 3]));

			vertices.push_back(v);
		}

		std::vector<unsigned int> indices = jmesh["indices"];

		gameObject->AddChild(name, std::make_shared<Mesh>(vertices, indices, Shader()));
	}

	return gameObject;
}
