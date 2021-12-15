#include "Importer.h"

#include <iostream>

void Importer::ImportGLTF(const std::string& name, const std::string& path)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	std::string fileType = path.substr(path.find_last_of(".") + 1);

	// parse GLTF
	bool ret = false;
	if (std::strcmp(fileType.c_str(), "gltf"))
	{
		ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
	}
	else if (std::strcmp(fileType.c_str(), "glb"))
	{
		ret = loader.LoadBinaryFromFile(&model, &err, &warn, path);
	}
	else
	{
		std::cout << "IMPORTER::ERROR::Invalid GLTF format!" << std::endl;
	}

	if (!warn.empty()) {
		std::cout << "IMPORTER::WARNING::" << warn << std::endl;
	}

	if (!err.empty()) {
		std::cout << "IMPORTER::ERROR::" << err << std::endl;
	}
	if (!ret) {
		std::cout << "IMPORTER::ERROR::Failed to parse GLTF!" << std::endl;
	}

	// meshes
	for (tinygltf::Mesh& mesh : model.meshes)
	{
		std::vector<glm::vec3> vertexPositions;
		std::vector<glm::vec3> vertexNormals;
		std::vector<glm::vec2> vertexTextureCoords;
		std::vector<glm::vec4> vertexColors;
		std::vector<glm::ivec4> vertexJoints;
		std::vector<glm::vec4> vertexWeights;

		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			getVertexPositions(model, primitive, vertexPositions);
			getVertexNormals(model, primitive, vertexNormals);
			getVertexTextureCoords(model, primitive, vertexTextureCoords);
			getVertexColors(model, primitive, vertexColors);
			getVertexJoints(model, primitive, vertexJoints);
			getVertexWeights(model, primitive, vertexWeights);
		}
	}
}

void Importer::getVertexPositions(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<glm::vec3>& vertexPositions)
{
	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["POSITION"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* positions = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	// convert to vec3 and store in vPos vector
	for (unsigned int i = 0; i < accessor.count; i++)
	{
		vertexPositions.push_back(glm::vec3(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]));
	}
}

void Importer::getVertexNormals(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<glm::vec3>& vertexNormals)
{
	if (primitive.attributes.count("NORMAL") == 0)
	{
		std::cout << "IMPORTER::WARNING::Mesh primitve has no normals attribute!";	// may need to change to error
		return;
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["NORMAL"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* normals = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	// convert to vec3 and store in vNorm vector
	for (unsigned int i = 0; i < accessor.count; i++)
	{
		vertexNormals.push_back(glm::vec3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]));
	}
}

void Importer::getVertexTextureCoords(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<glm::vec2>& vertexTextureCoords)
{
	if (primitive.attributes.count("TEXCOORD_0") == 0)
	{
		std::cout << "IMPORTER::INFO::Mesh primitve has no texture coordinates attribute";
		return;
	}

	if (primitive.attributes.count("TEXCOORD_1") > 0 || primitive.attributes.count("TEXCOORD_2") > 0 || primitive.attributes.count("TEXCOORD_3") > 0)
	{
		std::cout << "IMPORTER::WARNING::More than one texture coordinate per vertex is not supported!";
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["TEXCOORD_0"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* texcoords = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	// convert to vec2 and store in vTexCoord vector
	for (unsigned int i = 0; i < accessor.count; i++)
	{
		vertexTextureCoords.push_back(glm::vec2(texcoords[i * 2 + 0], texcoords[i * 2 + 1]));
	}
}

void Importer::getVertexColors(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<glm::vec4>& vertexColors)
{
	if (primitive.attributes.count("COLOR_0") == 0)
	{
		std::cout << "IMPORTER::INFO::Mesh primitve has no colors attribute";
		return;
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["COLOR_0"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* colors = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	// convert to vec4 and store in vNorm vector
	for (unsigned int i = 0; i < accessor.count; i++)
	{
		vertexColors.push_back(glm::vec4(colors[i * 4 + 0], colors[i * 4 + 1], colors[i * 4 + 2], colors[i * 4 + 3]));
	}
}

void Importer::getVertexJoints(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<glm::ivec4>& vertexJoints)
{
	if (primitive.attributes.count("JOINTS_0") == 0)
	{
		std::cout << "IMPORTER::INFO::Mesh primitve has no joints attribute";
		return;
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["JOINTS_0"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	int* joints = reinterpret_cast<int*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	// convert to vec4 and store in vJoints vector
	for (unsigned int i = 0; i < accessor.count; i++)
	{
		vertexJoints.push_back(glm::vec4(joints[i * 4 + 0], joints[i * 4 + 1], joints[i * 4 + 2], joints[i * 4 + 3]));
	}
}

void Importer::getVertexWeights(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<glm::vec4>& vertexWeights)
{
	if (primitive.attributes.count("WEIGHTS_0") == 0)
	{
		std::cout << "IMPORTER::INFO::Mesh primitve has no weights attribute";
		return;
	}

	tinygltf::Accessor& accessor = model.accessors[primitive.attributes["JOINTS_0"]];
	tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	float* weights = reinterpret_cast<float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

	// convert to vec4 and store in vWeights vector
	for (unsigned int i = 0; i < accessor.count; i++)
	{
		vertexWeights.push_back(glm::vec4(weights[i * 4 + 0], weights[i * 4 + 1], weights[i * 4 + 2], weights[i * 4 + 3]));
	}
}