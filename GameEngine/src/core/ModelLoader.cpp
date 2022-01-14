#include "ModelLoader.h"

#include <fstream>
//#include <iostream>

#include "glad/gl.h"
#include "stb/stb_image.h"

#include "ProjectManager.h"
#include "Log.h"
#include "../renderer/Mesh.h"

int loadTexture(const std::string& textureName)
{
	auto it = ModelLoader::TextureCache.find(textureName);

	if (it != ModelLoader::TextureCache.end())
	{
		return it->second;
	}

	if (textureName == "")
	{
		return -1;	// TODO: make return default full white texture index
	}

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

	unsigned char* data = stbi_load((ProjectManager::GetTexturesPath() + textureName).c_str(), &width, &height, &nrChannels, 0);
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
			LOG_ERROR("MODEL_LOADER::Texture format not supported!")
				return -1;	// TODO: make return default full white texture index
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		LOG_ERROR("MODEL_LOADER::Failed to load texture!")
			return -1;	// TODO: make return default full white texture index
	}
	stbi_image_free(data);

	ModelLoader::TextureCache[textureName] = texture;
	return texture;
}
Skeleton createSkeleton(json& j)
{
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

						transform = glm::translate(glm::mat4(1.0f), translation) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);

						keyframes[std::stof(time)][i] = transform;
					}
				}
			}
			animations[name] = Animation(keyframes);
		}
	}
	else
	{
		LOG_WARN("MODEL_LOADER::Model has no skeleton")
	}

	return Skeleton(skeletonJoints, animations);
}
Mesh createMesh(json& jmesh)
{
	// buffer data
	std::vector<float> vertFloats = jmesh["vertices"];

	std::vector<Vertex> vertices;
	int arraySize = (int)vertFloats.size() / 20; // 20 is num of floats in a vertex
	vertices.reserve(arraySize);

	for (int i = 0; i < arraySize; i++)
	{
		vertices.push_back(Vertex(glm::vec3(vertFloats[i * 20 + 0], vertFloats[i * 20 + 1], vertFloats[i * 20 + 2]),
			glm::vec3(vertFloats[i * 20 + 3], vertFloats[i * 20 + 4], vertFloats[i * 20 + 5]),
			glm::vec2(vertFloats[i * 20 + 6], vertFloats[i * 20 + 7]),
			glm::vec4(vertFloats[i * 20 + 8], vertFloats[i * 20 + 9], vertFloats[i * 20 + 10], vertFloats[i * 20 + 11]),
			glm::ivec4(vertFloats[i * 20 + 12], vertFloats[i * 20 + 13], vertFloats[i * 20 + 14], vertFloats[i * 20 + 15]),
			glm::vec4(vertFloats[i * 20 + 16], vertFloats[i * 20 + 17], vertFloats[i * 20 + 18], vertFloats[i * 20 + 19])));
	}

	std::vector<unsigned int> indices = jmesh["indices"];

	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);	// vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);	// vertex normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);	// vertex texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

	glBindVertexArray(0);	// unbind VAO

	auto bufferData = std::make_shared<BufferData>(VAO, indices.size());


	// material
	std::shared_ptr<Material> material = std::make_shared<Material>();
	std::string path;
	material->BaseColorTexture = loadTexture(jmesh["baseColorTexture"]);

	path = jmesh["metallicRoughnessTexture"];
	material->MetallicRoughnessTexture = loadTexture(path);

	path = jmesh["emissiveTexture"];
	material->EmissiveTexture = loadTexture(path);

	path = jmesh["normalTexture"];
	material->Normaltexture = loadTexture(path);

	path = jmesh["occlusionTexture"];
	material->OcclusionTexture = loadTexture(path);

	std::vector<float> baseColor = jmesh["baseColorFactor"];
	material->BaseColorFactor = glm::vec4(baseColor[0], baseColor[1], baseColor[2], baseColor[3]);

	material->MetallicFactor = jmesh["metallicFactor"];
	material->RoughnessFactor = jmesh["roughnessFactor"];

	std::vector<float> emissiveColor = jmesh["emissiveFactor"];
	material->EmissiveFactor = glm::vec3(emissiveColor[0], emissiveColor[1], emissiveColor[2]);


	// transform
	std::vector<float> transformVec = jmesh["transform"];
	glm::mat4 transform = { transformVec[0], transformVec[4], transformVec[8], transformVec[12],
							transformVec[1], transformVec[5], transformVec[9], transformVec[13],
							transformVec[2], transformVec[6], transformVec[10], transformVec[14],
							transformVec[3], transformVec[7], transformVec[11], transformVec[15] };


	return Mesh(bufferData, nullptr, material, std::make_shared<Shader>(), transform);
}

std::map<std::string, int> ModelLoader::TextureCache;	// TODO: Move to TextureLibrary
std::map<std::string, Model> ModelLoader::ModelCache;

std::shared_ptr<GameObject> ModelLoader::LoadModel(const std::string& modelPath)
{
	std::string modelName = modelPath.substr(modelPath.find_last_of("/") + 1);
	modelName.resize(modelName.find_last_of("."));

	// check if model is in cache
	auto it = ModelCache.find(modelName);
	if (it != ModelCache.end())
	{
		Model& model = it->second;
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();

		auto skeleton = std::make_shared<Skeleton>(model.m_Skeleton);
		gameObject->AddChild("Skeleton", skeleton);

		for (const auto& [name, savedMesh] : model.m_Meshes)
		{
			auto mesh = std::make_shared<Mesh>(savedMesh);
			mesh->m_Skeleton = skeleton;
			gameObject->AddChild(name, mesh);
		}

		return gameObject;
	}

	// load from file
	std::streamsize size = std::filesystem::file_size(modelPath);
	std::vector<char> buffer;
	buffer.resize(size);

	std::ifstream fileIn = std::ifstream(modelPath, std::ios::in | std::ios::binary);
	fileIn.read(&buffer[0], size);
	fileIn.close();

	json j = json::from_bson(buffer);

	std::map<std::string, Mesh> meshes;
	for (auto& [name, jmesh] : j["meshes"].items())
	{
		meshes[name] = createMesh(jmesh);
	}

	Model model = Model(meshes, createSkeleton(j));

	ModelCache[modelName] = model;

	return LoadModel(modelPath);
}
