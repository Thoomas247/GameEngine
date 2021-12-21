#include "AssetManager.h"

#include <map>
#include <fstream>
#include <iostream>

#include "glad/gl.h"
#include "glm/glm.hpp"
#include "stb/stb_image.h"
#include "json/json.hpp"
using json = nlohmann::json;

#include "ProjectManager.h"
#include "AnimationManager.h"
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

	
	std::vector<Joint> skeletonJoints;
	std::map<std::string, Animation> animations;
	if (!j["joints"].is_null())
	{
		// skeleton
		glm::mat4 matrix;
		for (unsigned int i = 0; i < j["joints"].size(); i++)
		{
			std::vector<float> vec = j["joints"][i]["inverseBindMatrix"];
			matrix = glm::mat4(vec[0], vec[1], vec[2], vec[3],
				vec[4], vec[5], vec[6], vec[7],
				vec[8], vec[9], vec[10], vec[11],
				vec[12], vec[13], vec[14], vec[15]);


			skeletonJoints.push_back(Joint(j["joints"][i]["parentID"], matrix));
		}

		// animations
		for (auto& [name, anim] : j["animations"].items())
		{
			std::map<float, std::map<int, glm::mat4>> keyframes;	// timestamp, joint id, matrix transform
			glm::mat4 transform;
			for (auto& [time, targets] : anim.items())
			{
				for (int i = 0; i < targets.size(); i++)
				{
					json& target = targets[i];

					if (target.is_null())
					{
						transform = glm::mat4(1.0f);
					}
					else
					{
						glm::vec3 translation = glm::vec3(target["translation"][0], target["translation"][1], target["translation"][2]);
						glm::quat rotation = glm::quat(target["rotation"][0], target["rotation"][1], target["rotation"][2], target["rotation"][3]);
						glm::vec3 scale = glm::vec3(target["scale"][0], target["scale"][1], target["scale"][2]);

						transform = glm::mat4(1.0f);
						transform = glm::scale(transform, scale);
						transform = glm::mat4_cast(rotation) * transform;
						transform = glm::translate(transform, translation);

						keyframes[std::stof(time)][i] = transform;
					}
				}
			}
			animations[name] = Animation(keyframes);
		}
	}
	else
	{
		std::cout << "ASSETMANAGER::WARNING::Model has no skeleton" << std::endl;
	}

	std::shared_ptr<Skeleton> skeleton = std::make_shared<Skeleton>(skeletonJoints, animations);
	AnimationManager::Skeletons.push_back(skeleton);

	// meshes
	for (auto& [name, jmesh] : j["meshes"].items())
	{
		// vertices
		std::vector<float> positions = jmesh["positions"];
		std::vector<float> normals = jmesh["normals"];
		std::vector<float> texCoords = jmesh["texCoords"];
		std::vector<float> colors = jmesh["colors"];
		std::vector<int> joints = jmesh["joints"];
		std::vector<float> weights = jmesh["weights"];

		std::vector<Vertex> vertices;
		int arraySize = (int)positions.size() / 3; // 3 is num of components in the position vectors
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

		// indices
		std::vector<unsigned int> indices = jmesh["indices"];

		// material
		Material material;
		std::string path = jmesh["material"]["baseColorTexture"];
		material.BaseColorTexture = loadTexture(path);

		path = jmesh["material"]["metallicRoughnessTexture"];
		material.MetallicRoughnessTexture = loadTexture(path);

		path = jmesh["material"]["emissiveTexture"];
		material.EmissiveTexture = loadTexture(path);

		path = jmesh["material"]["normalTexture"];
		material.Normaltexture = loadTexture(path);

		path = jmesh["material"]["occlusionTexture"];
		material.OcclusionTexture = loadTexture(path);

		std::vector<float> baseColor = jmesh["material"]["baseColorFactor"];
		material.BaseColorFactor = glm::vec4(baseColor[0], baseColor[1], baseColor[2], baseColor[3]);

		material.MetallicFactor = jmesh["material"]["metallicFactor"];
		material.RoughnessFactor = jmesh["material"]["roughnessFactor"];

		std::vector<float> emissiveColor = jmesh["material"]["emissiveFactor"];
		material.EmissiveFactor = glm::vec3(emissiveColor[0], emissiveColor[1], emissiveColor[2]);

		gameObject->AddChild(name, std::make_shared<Mesh>(vertices, indices, Shader(), material, skeleton));
	}

	return gameObject;
}

int AssetManager::loadTexture(const std::string& name)
{
	if (name == "")
	{
		return -1;	// TODO: make return default full white texture index
	}

	std::string path = ProjectManager::ProjectPath + ProjectManager::DefaultTexturesPath + name;

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format = GL_RED;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		else
		{
			std::cout << "ASSETMANAGER::ERROR::Texture format not supported!" << std::endl;
			return -1;	// TODO: make return default full white texture index
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ASSETMANAGER::ERROR::Failed to load texture!" << std::endl;
		return -1;	// TODO: make return default full white texture index
	}
	stbi_image_free(data);
	return texture;
}
