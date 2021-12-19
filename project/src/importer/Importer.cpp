#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "Importer.h"

#include <iostream>
#include <filesystem>

#include "json/json.hpp"

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

	// MESHES
	for (tinygltf::Mesh& mesh : model.meshes)
	{
		tinygltf::Primitive& primitive = mesh.primitives[0];

		if (primitive.mode != TINYGLTF_MODE_TRIANGLES && primitive.mode != TINYGLTF_MODE_TRIANGLE_FAN && primitive.mode != TINYGLTF_MODE_TRIANGLE_STRIP)
		{
			std::cout << "IMPORTER::ERROR::Primitive mode not supported!" << std::endl;
			return;
		}

		j[mesh.name]["positions"] = getVertexPositions(model, primitive);
		j[mesh.name]["normals"] = getVertexNormals(model, primitive);
		j[mesh.name]["texCoords"] = getVertexTextureCoords(model, primitive);
		j[mesh.name]["colors"] = getVertexColors(model, primitive);
		j[mesh.name]["joints"] = getVertexJoints(model, primitive);
		j[mesh.name]["weights"] = getVertexWeights(model, primitive);

		j[mesh.name]["indices"] = getIndices(model, primitive);
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
		tinygltf::Accessor& accessor = model.accessors[skin.inverseBindMatrices];
		tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
		tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

		float* matrices = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

		for (unsigned int i = 0; i < accessor.count; i++)
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
	}

	// ANIMATIONS
	model.animations;

	// WRITE to file
	std::ofstream fileOut = std::ofstream("C:/Users/TM1/source/repos/GameEngine/project/TestModel.GEM", std::ios::out | std::ios::binary);
	std::vector<unsigned char> dataVec = json::to_bson(j);
	fileOut.write(reinterpret_cast<const char*>(dataVec.data()), dataVec.size());
	fileOut.close();
	__debugbreak();

	/*	READ FILE: Use simdjson in actual implementation, this is for TESTING only
	std::streamsize size = std::filesystem::file_size("C:/Users/TM1/source/repos/GameEngine/project/TestModel.GEM");
	std::vector<char> buffer;
	buffer.reserve(size);

	std::ifstream fileIn = std::ifstream("C:/Users/TM1/source/repos/GameEngine/project/TestModel.GEM", std::ios::in | std::ios::binary);
	fileIn.read(buffer.data(), size);
	fileIn.close();

	json j2 = json::from_bson(buffer);
	*/
}

std::vector<float> Importer::getVertexPositions(tinygltf::Model& model, tinygltf::Primitive& primitive)
{
	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["POSITION"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* positions = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	std::vector<float> vertexPositions;
	vertexPositions.reserve(accessor.count);

	for (unsigned int i = 0; i < accessor.count; i++)
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
		return std::vector<float>();
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["NORMAL"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* normals = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	std::vector<float> vertexNormals;
	vertexNormals.reserve(accessor.count);

	for (unsigned int i = 0; i < accessor.count; i++)
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
		return std::vector<float>();
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
	vertexTextureCoords.reserve(accessor.count);

	for (unsigned int i = 0; i < accessor.count; i++)
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
		return std::vector<float>();
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["COLOR_0"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* colors = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	std::vector<float> vertexColors;
	vertexColors.reserve(accessor.count);

	for (unsigned int i = 0; i < accessor.count; i++)
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
		return std::vector<unsigned short>();
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["JOINTS_0"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	unsigned short* joints = reinterpret_cast<unsigned short*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	std::vector<unsigned short> vertexJoints;
	vertexJoints.reserve(accessor.count);

	for (unsigned int i = 0; i < accessor.count; i++)
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
		return std::vector<float>();
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["WEIGHTS_0"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* weights = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	std::vector<float> vertexWeights;
	vertexWeights.reserve(accessor.count);

	for (unsigned int i = 0; i < accessor.count; i++)
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