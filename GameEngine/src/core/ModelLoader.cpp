#include "ModelLoader.h"

#include <fstream>
#include <iostream>

#include "glad/gl.h"
#include "stb/stb_image.h"

#include "ProjectManager.h"
#include "../renderer/Mesh.h"

std::map<std::string, int> ModelLoader::g_TextureCache;	// TODO: Move to TextureLibrary
std::map<std::string, Model> ModelLoader::g_ModelCache;

std::shared_ptr<GameObject> ModelLoader::LoadModel(const std::string& modelPath)
{
	std::string modelName = modelPath.substr(modelPath.find_last_of("/") + 1);
	modelName.resize(modelName.find_last_of("."));

	// check if model is in cache
	auto it = g_ModelCache.find(modelName);
	if (it != g_ModelCache.end())
	{
		Model& model = it->second;
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();

		std::shared_ptr<Skeleton> skeleton = std::make_shared<Skeleton>(model.m_Skeleton.m_Joints, model.m_Skeleton.m_Animations);
		gameObject->AddChild("Skeleton", skeleton);

		for (const Model::MeshCache& meshCache : model.m_Meshes)
		{
			gameObject->AddChild(meshCache.m_Name, std::make_shared<Mesh>(meshCache.m_RenderData, meshCache.m_MeshData, skeleton, meshCache.m_Transform));
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

	Model model;
	model.m_Skeleton = createSkeleton(j);

	for (auto& [name, jmesh] : j["meshes"].items())
	{
		model.m_Meshes.push_back(Model::MeshCache(name, createRenderData(jmesh), createMeshData(jmesh), createTransform(jmesh)));
	}

	g_ModelCache[modelName] = model;

	return LoadModel(modelPath);
}

int ModelLoader::loadTexture(const std::string& textureName)
{
	auto it = g_TextureCache.find(textureName);

	if (it != g_TextureCache.end())
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
			std::cout << "MODEL_LOADER::ERROR::Texture format not supported!" << std::endl;
			return -1;	// TODO: make return default full white texture index
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "MODEL_LOADER::ERROR::Failed to load texture!" << std::endl;
		return -1;	// TODO: make return default full white texture index
	}
	stbi_image_free(data);

	g_TextureCache[textureName] = texture;
	return texture;
}

RenderData ModelLoader::createRenderData(json& jmesh)
{
	// TODO: Clean up

	// material
	std::shared_ptr<Material> material = std::make_shared<Material>();
	std::string path = jmesh["baseColorTexture"];
	material->BaseColorTexture = loadTexture(path);

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

	return RenderData(material, std::make_shared<Shader>());	// return default shader for now
}

std::shared_ptr<MeshData> ModelLoader::createMeshData(json& jmesh)
{
	// vertices
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

	// indices
	std::vector<unsigned int> indices = jmesh["indices"];

	// create buffers
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

	glBindVertexArray(0);	// unbind VAO

	return std::make_shared<MeshData>(VAO, (unsigned int)indices.size());
}

glm::mat4 ModelLoader::createTransform(json& jmesh)
{
	std::vector<float> transformVec = jmesh["transform"];
	glm::mat4 transform = { transformVec[0], transformVec[4], transformVec[8], transformVec[12],
							transformVec[1], transformVec[5], transformVec[9], transformVec[13],
							transformVec[2], transformVec[6], transformVec[10], transformVec[14],
							transformVec[3], transformVec[7], transformVec[11], transformVec[15] };

	return transform;
}

Model::SkeletonCache ModelLoader::createSkeleton(json& j)
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
		std::cout << "MODEL_LOADER::WARNING::Model has no skeleton" << std::endl;
	}

	return Model::SkeletonCache(skeletonJoints, animations);
}