#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "Importer.h"

#include <iostream>
#include <filesystem>

#include "json/json.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "../core/ProjectManager.h"

using json = nlohmann::json;

void Importer::ImportGLTF(const std::string& name, const std::string& path)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	std::string fileType = path.substr(path.find_last_of(".") + 1);

	// parse GLTF
	bool ret = false;
	if (std::strcmp(fileType.c_str(), "gltf") == 0)
	{
		ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
	}
	else if (std::strcmp(fileType.c_str(), "glb") == 0)
	{
		ret = loader.LoadBinaryFromFile(&model, &err, &warn, path);
	}
	else
	{
		std::cout << "IMPORTER::ERROR::Invalid GLTF format!" << std::endl;
		return;
	}

	if (!warn.empty()) {
		std::cout << "IMPORTER::WARNING::" << warn << std::endl;
	}
	if (!err.empty()) {
		std::cout << "IMPORTER::ERROR::" << err << std::endl;
	}
	if (!ret) {
		std::cout << "IMPORTER::ERROR::Failed to parse GLTF!" << std::endl;
		return;
	}

	json j;

	// TEXTURES
	std::map<int, std::string> textureDict;
	for (int i = 0; i < model.textures.size(); i++)
	{
		tinygltf::Texture& texture = model.textures[i];
		std::string& uri = model.images[texture.source].uri;
		if (!uri.empty())
		{
			std::string texturePath = path.substr(0, path.find_last_of("/") + 1) + uri;
			std::string destPath = ProjectManager::ProjectPath + ProjectManager::DefaultTexturesPath + name + "_textures";

			std::filesystem::create_directories(destPath);

			destPath += uri.substr(uri.find_last_of("/"));

			if (!std::filesystem::copy_file(texturePath, destPath, std::filesystem::copy_options::overwrite_existing))
			{
				std::cout << "IMPORTER::ERROR::Texture could not be copied! (" << uri << ")" << std::endl;
			}
			else
			{
				textureDict[i] = name + "_textures" + uri.substr(uri.find_last_of("/"));	// store relative path of texture
			}

		}
		else
		{
			std::cout << "IMPORTER::WARNING::Texture does not have uri, not importing" << std::endl;
		}
	}

	// MESHES
	/*
	std::map<int, std::vector<double>> meshNodeTransforms;
	for (tinygltf::Node& node : model.nodes)
	{
		if (node.mesh != -1)
		{
			if (node.matrix.size() != 0)
			{
				meshNodeTransforms[node.mesh] = node.matrix;
			}
			else if (node.translation.size() != 0)
			{
				glm::vec3 translation = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
				glm::quat rotation = glm::quat(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
				glm::vec3 scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);

				glm::mat4 localtransform = glm::scale(glm::mat4(1.0f), scale);
				localtransform = glm::mat4_cast(rotation) * localtransform;
				localtransform = glm::translate(localtransform, translation);

				meshNodeTransforms[node.mesh] = { localtransform[0][0], localtransform[1][0], localtransform[2][0], localtransform[3][0],
												localtransform[0][1], localtransform[1][1], localtransform[2][1], localtransform[3][1],
												localtransform[0][2], localtransform[1][2], localtransform[2][2], localtransform[3][2],
												localtransform[0][3], localtransform[1][3], localtransform[2][3], localtransform[3][3] };
			}
			else
			{
				meshNodeTransforms[node.mesh] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
			}
		}
	}
	*/

	for (tinygltf::Mesh& mesh : model.meshes)
	{
		tinygltf::Primitive& primitive = mesh.primitives[0];
		
		if (primitive.mode != TINYGLTF_MODE_TRIANGLES && primitive.mode != TINYGLTF_MODE_TRIANGLE_FAN && primitive.mode != TINYGLTF_MODE_TRIANGLE_STRIP)
		{
			std::cout << "IMPORTER::ERROR::Primitive mode not supported!" << std::endl;
			return;
		}

		// vertices
		j["meshes"][mesh.name]["positions"] = getVertexPositions(model, primitive);
		j["meshes"][mesh.name]["normals"] = getVertexNormals(model, primitive);
		j["meshes"][mesh.name]["texCoords"] = getVertexTextureCoords(model, primitive);
		j["meshes"][mesh.name]["colors"] = getVertexColors(model, primitive);
		j["meshes"][mesh.name]["joints"] = getVertexJoints(model, primitive);
		j["meshes"][mesh.name]["weights"] = getVertexWeights(model, primitive);

		// indices
		j["meshes"][mesh.name]["indices"] = getIndices(model, primitive);

		// material
		int& materialIndex = primitive.material;
		tinygltf::Material& material = model.materials[materialIndex];

		j["meshes"][mesh.name]["material"]["baseColorFactor"] = material.pbrMetallicRoughness.baseColorFactor;
		j["meshes"][mesh.name]["material"]["baseColorTexture"] = textureDict[material.pbrMetallicRoughness.baseColorTexture.index];
		j["meshes"][mesh.name]["material"]["metallicFactor"] = material.pbrMetallicRoughness.metallicFactor;
		j["meshes"][mesh.name]["material"]["roughnessFactor"] = material.pbrMetallicRoughness.roughnessFactor;
		j["meshes"][mesh.name]["material"]["metallicRoughnessTexture"] = textureDict[material.pbrMetallicRoughness.metallicRoughnessTexture.index];
		j["meshes"][mesh.name]["material"]["emissiveFactor"] = material.emissiveFactor;
		j["meshes"][mesh.name]["material"]["emissiveTexture"] = textureDict[material.emissiveTexture.index];
		j["meshes"][mesh.name]["material"]["normalTexture"] = textureDict[material.normalTexture.index];
		j["meshes"][mesh.name]["material"]["occlusionTexture"] = textureDict[material.occlusionTexture.index];
	}

	// JOINTS
	if (model.skins.size() > 1)
	{
		std::cout << "IMPORTER::WARNING::More than one skin not supported" << std::endl;
	}

	if (model.skins.size() == 0)
	{
		std::cout << "IMPORTER::WARNING::Model has no skinning information" << std::endl;
	}

	else
	{
		tinygltf::Skin skin = model.skins[0];

		// read invBindMatrices and assign them to their joints
		tinygltf::Accessor& matrixAccessor = model.accessors[skin.inverseBindMatrices];
		tinygltf::BufferView& matrixBufferView = model.bufferViews[matrixAccessor.bufferView];
		tinygltf::Buffer& matrixBuffer = model.buffers[matrixBufferView.buffer];

		float* matrices = reinterpret_cast<float*>(&matrixBuffer.data[matrixBufferView.byteOffset + matrixAccessor.byteOffset]);

		for (unsigned int i = 0; i < matrixAccessor.count; i++)
		{
			j["joints"][i]["inverseBindMatrix"] = { matrices[i * 16 + 0], matrices[i * 16 + 1], matrices[i * 16 + 2], matrices[i * 16 + 3],
				matrices[i * 16 + 4], matrices[i * 16 + 5], matrices[i * 16 + 6], matrices[i * 16 + 7],
				matrices[i * 16 + 8], matrices[i * 16 + 9], matrices[i * 16 + 10], matrices[i * 16 + 11],
				matrices[i * 16 + 12], matrices[i * 16 + 13], matrices[i * 16 + 14], matrices[i * 16 + 15] };
		}

		// dict to translate from gltf indexes to own indexes
		std::map<int, int> indexDict;
		for (unsigned int i = 0; i < skin.joints.size(); i++)
		{
			indexDict[skin.joints[i]] = i;
		}

		// set joint parents
		for (int& jointIndex : skin.joints)
		{
			for (int& childIndex : model.nodes[jointIndex].children)
			{
				j["joints"][indexDict[childIndex]]["parentID"] = indexDict[jointIndex];
			}
		}

		// ANIMATIONS
		for (tinygltf::Animation& anim : model.animations)
		{
			for (tinygltf::AnimationChannel& channel : anim.channels)
			{
				tinygltf::AnimationSampler& sampler = anim.samplers[channel.sampler];

				// input
				tinygltf::Accessor& inputAccessor = model.accessors[sampler.input];
				tinygltf::BufferView& inputBufferView = model.bufferViews[inputAccessor.bufferView];
				tinygltf::Buffer& inputBuffer = model.buffers[inputBufferView.buffer];
				float* times = reinterpret_cast<float*>(&inputBuffer.data[inputBufferView.byteOffset + inputAccessor.byteOffset]);
				// output
				tinygltf::Accessor& outputAccessor = model.accessors[sampler.output];
				tinygltf::BufferView& outputBufferView = model.bufferViews[outputAccessor.bufferView];
				tinygltf::Buffer& outputBuffer = model.buffers[outputBufferView.buffer];
				float* animatedProperty = reinterpret_cast<float*>(&outputBuffer.data[outputBufferView.byteOffset + outputAccessor.byteOffset]);

				for (unsigned int i = 0; i < inputAccessor.count; i++)
				{
					if (channel.target_path == "translation")
					{
						j["animations"][anim.name][std::to_string(times[i])][indexDict[channel.target_node]]["translation"] = { animatedProperty[i * 3 + 0], animatedProperty[i * 3 + 1], animatedProperty[i * 3 + 2] };
					}
					else if (channel.target_path == "rotation")
					{
						j["animations"][anim.name][std::to_string(times[i])][indexDict[channel.target_node]]["rotation"] = { animatedProperty[i * 4 + 0], animatedProperty[i * 4 + 1], animatedProperty[i * 4 + 2], animatedProperty[i * 4 + 3] };
					}
					else if (channel.target_path == "scale")
					{
						j["animations"][anim.name][std::to_string(times[i])][indexDict[channel.target_node]]["scale"] = { animatedProperty[i * 3 + 0], animatedProperty[i * 3 + 1], animatedProperty[i * 3 + 2] };;
					}
				}
			}
		}
	}

	// WRITE to file - importing done
	std::string modelDestPath = ProjectManager::ProjectPath + ProjectManager::DefaultModelPath;
	std::filesystem::create_directories(modelDestPath);
	modelDestPath += name + ".GEM";

	std::ofstream fileOut = std::ofstream(modelDestPath, std::ios::out | std::ios::binary);
	std::vector<unsigned char> dataVec = json::to_bson(j);
	fileOut.write(reinterpret_cast<const char*>(dataVec.data()), dataVec.size());
	fileOut.close();
}

std::vector<float> Importer::getVertexPositions(tinygltf::Model& model, tinygltf::Primitive& primitive)
{
	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["POSITION"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* positions = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	std::vector<float> vertexPositions;
	vertexPositions.reserve(accessor.count * TINYGLTF_TYPE_VEC3);

	for (unsigned int i = 0; i < accessor.count * TINYGLTF_TYPE_VEC3; i++)
	{
		vertexPositions.push_back(positions[i]);
	}

	return vertexPositions;
}

std::vector<float> Importer::getVertexNormals(tinygltf::Model& model, tinygltf::Primitive& primitive)
{
	if (primitive.attributes.count("NORMAL") == 0)
	{
		std::cout << "IMPORTER::WARNING::Mesh has no normals attribute!" << std::endl;	// may need to change to error
		tinygltf::Accessor& accessor = model.accessors[primitive.attributes["POSITION"]];
		return std::vector<float>(accessor.count * TINYGLTF_TYPE_VEC3, 1.0f);
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["NORMAL"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* normals = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	std::vector<float> vertexNormals;
	vertexNormals.reserve(accessor.count * TINYGLTF_TYPE_VEC3);

	for (unsigned int i = 0; i < accessor.count * TINYGLTF_TYPE_VEC3; i++)
	{
		vertexNormals.push_back(normals[i]);
	}

	return vertexNormals;
}

std::vector<float> Importer::getVertexTextureCoords(tinygltf::Model& model, tinygltf::Primitive& primitive)
{
	if (primitive.attributes.count("TEXCOORD_0") == 0)
	{
		std::cout << "IMPORTER::INFO::Mesh has no texture coordinates attribute" << std::endl;
		tinygltf::Accessor& accessor = model.accessors[primitive.attributes["POSITION"]];
		return std::vector<float>(accessor.count * TINYGLTF_TYPE_VEC2, 1.0f);
	}

	if (primitive.attributes.count("TEXCOORD_1") > 0 || primitive.attributes.count("TEXCOORD_2") > 0 || primitive.attributes.count("TEXCOORD_3") > 0)
	{
		if (primitive.attributes["TEXCOORD_1"] != primitive.attributes["TEXCOORD_0"])	// if 0 and 1 are same, then so are all the others
		{
			std::cout << "IMPORTER::WARNING::More than one texture coordinate per vertex is not supported!" << std::endl;
		}
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["TEXCOORD_0"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* texcoords = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	std::vector<float> vertexTextureCoords;
	vertexTextureCoords.reserve(accessor.count * TINYGLTF_TYPE_VEC2);

	for (unsigned int i = 0; i < accessor.count * TINYGLTF_TYPE_VEC2; i++)
	{
		vertexTextureCoords.push_back(texcoords[i]);
	}

	return vertexTextureCoords;
}

std::vector<float> Importer::getVertexColors(tinygltf::Model& model, tinygltf::Primitive& primitive)
{
	if (primitive.attributes.count("COLOR_0") == 0)
	{
		std::cout << "IMPORTER::INFO::Mesh has no colors attribute" << std::endl;
		tinygltf::Accessor& accessor = model.accessors[primitive.attributes["POSITION"]];
		return std::vector<float>(accessor.count * TINYGLTF_TYPE_VEC4, 1.0f);
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["COLOR_0"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* colors = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	std::vector<float> vertexColors;
	vertexColors.reserve(accessor.count * TINYGLTF_TYPE_VEC4);

	for (unsigned int i = 0; i < accessor.count * TINYGLTF_TYPE_VEC4; i++)
	{
		vertexColors.push_back(colors[i]);
	}

	return vertexColors;
}

std::vector<unsigned short> Importer::getVertexJoints(tinygltf::Model& model, tinygltf::Primitive& primitive)
{
	if (primitive.attributes.count("JOINTS_0") == 0)
	{
		std::cout << "IMPORTER::INFO::Mesh has no joints attribute" << std::endl;
		tinygltf::Accessor& accessor = model.accessors[primitive.attributes["POSITION"]];
		return std::vector<unsigned short>(accessor.count * TINYGLTF_TYPE_VEC4, 0);
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["JOINTS_0"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	unsigned short* joints = reinterpret_cast<unsigned short*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	std::vector<unsigned short> vertexJoints;
	vertexJoints.reserve(accessor.count * TINYGLTF_TYPE_VEC4);

	for (unsigned int i = 0; i < accessor.count * TINYGLTF_TYPE_VEC4; i++)
	{
		vertexJoints.push_back(joints[i]);
	}

	return vertexJoints;
}

std::vector<float> Importer::getVertexWeights(tinygltf::Model& model, tinygltf::Primitive& primitive)
{
	if (primitive.attributes.count("WEIGHTS_0") == 0)
	{
		std::cout << "IMPORTER::INFO::Mesh has no weights attribute" << std::endl;
		tinygltf::Accessor& accessor = model.accessors[primitive.attributes["POSITION"]];
		return std::vector<float>(accessor.count * TINYGLTF_TYPE_VEC4, 0);
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["WEIGHTS_0"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* weights = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	std::vector<float> vertexWeights;
	vertexWeights.reserve(accessor.count * TINYGLTF_TYPE_VEC4);

	for (unsigned int i = 0; i < accessor.count * TINYGLTF_TYPE_VEC4; i++)
	{
		vertexWeights.push_back(weights[i]);
	}

	return vertexWeights;
}

std::vector<unsigned int> Importer::getIndices(tinygltf::Model& model, tinygltf::Primitive& primitive)
{
	tinygltf::Accessor& accessor = model.accessors[primitive.indices];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	unsigned int* indices = reinterpret_cast<unsigned int*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	std::vector<unsigned int> meshIndices;
	meshIndices.reserve(accessor.count);

	for (unsigned int i = 0; i < accessor.count; i++)
	{
		meshIndices.push_back(indices[i]);
	}

	return meshIndices;
}