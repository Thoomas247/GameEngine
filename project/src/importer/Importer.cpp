#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "Importer.h"

#include <iostream>
#include <filesystem>

#include "../core/ProjectManager.h"

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

	// textures
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

	// scenes
	tinygltf::Scene& scene = model.scenes[model.defaultScene];	// only one scene is supported
	std::map<int, glm::mat4> parentTransforms;	// index of child, compounded transform of parents
	std::map<std::string, int> nameToIndex;	// for applying transforms later

	// nodes
	for (int nodeIndex : scene.nodes)
	{
		tinygltf::Node node = model.nodes[nodeIndex];

		// transform
		glm::mat4 transform = getNodeTransform(node);

		for (int& childIndex : node.children)
		{
			auto it = parentTransforms.find(childIndex);

			if (it != parentTransforms.end())
			{
				it->second = transform * it->second;
			}

			else
			{
				parentTransforms[childIndex] = transform;
			}
		}

		// if node contains a mesh, make sure the transform is applied later
		if (node.mesh != -1)
		{
			nameToIndex[model.meshes[node.mesh].name] = nodeIndex;
		}
	}

	// meshes
	for (tinygltf::Mesh& mesh : model.meshes)
	{
		// vertices
		tinygltf::Primitive& primitive = mesh.primitives[0];

		if (primitive.mode != TINYGLTF_MODE_TRIANGLES && primitive.mode != TINYGLTF_MODE_TRIANGLE_FAN && primitive.mode != TINYGLTF_MODE_TRIANGLE_STRIP)
		{
			std::cout << "IMPORTER::ERROR::Primitive mode not supported!" << std::endl;
			return;
		}

		std::vector<float> positions = getVertexPositions(model, primitive);
		std::vector<float> normals = getVertexNormals(model, primitive);
		std::vector<float> texCoords = getVertexTextureCoords(model, primitive);
		std::vector<float> colors = getVertexColors(model, primitive);
		std::vector<unsigned short> joints = getVertexJoints(model, primitive);
		std::vector<float> weights = getVertexWeights(model, primitive);

		if (normals.size() == 0) 
		{
			normals = std::vector<float>(positions.size(), 1.0f);
		}
		if (texCoords.size() == 0)
		{
			texCoords = std::vector<float>((positions.size() / 3) * 2, 0.0f);
		}
		if (colors.size() == 0)
		{
			colors = std::vector<float>((positions.size() / 3) * 4, 1.0f);
		}
		if (joints.size() == 0)
		{
			joints = std::vector<unsigned short>((positions.size() / 3) * 4, 0);
		}
		if (weights.size() == 0)
		{
			weights = std::vector<float>((positions.size() / 3) * 4, 0.0f);
		}

		// interleave vertex data
		std::vector<float> vertices;
		vertices.reserve((positions.size() / 3) * 20);	// 20 floats per vertex
		for (int i = 0; i < (positions.size() / 3); i++)
		{
			vertices.push_back(positions[i * 3 + 0]);
			vertices.push_back(positions[i * 3 + 1]);
			vertices.push_back(positions[i * 3 + 2]);

			vertices.push_back(normals[i * 3 + 0]);
			vertices.push_back(normals[i * 3 + 1]);
			vertices.push_back(normals[i * 3 + 2]);

			vertices.push_back(texCoords[i * 2 + 0]);
			vertices.push_back(texCoords[i * 2 + 1]);

			vertices.push_back(colors[i * 4 + 0]);
			vertices.push_back(colors[i * 4 + 1]);
			vertices.push_back(colors[i * 4 + 2]);
			vertices.push_back(colors[i * 4 + 3]);

			vertices.push_back(joints[i * 4 + 0]);
			vertices.push_back(joints[i * 4 + 1]);
			vertices.push_back(joints[i * 4 + 2]);
			vertices.push_back(joints[i * 4 + 3]);

			vertices.push_back(weights[i * 4 + 0]);
			vertices.push_back(weights[i * 4 + 1]);
			vertices.push_back(weights[i * 4 + 2]);
			vertices.push_back(weights[i * 4 + 3]);
		}

		// check if mesh name already exists
		std::string meshName = getNewName(mesh.name, j["meshes"], 0);

		j["meshes"][meshName]["vertices"] = vertices;

		// indices
		std::vector<unsigned int> indices = getIndices(model, primitive);
		j["meshes"][meshName]["indices"] = indices;

		// material
		int& materialIndex = primitive.material;
		tinygltf::Material& material = model.materials[materialIndex];

		std::vector<double> baseColorFactor = material.pbrMetallicRoughness.baseColorFactor;
		std::string			baseColorTexture = textureDict[material.pbrMetallicRoughness.baseColorTexture.index];
		double				metallicFactor = material.pbrMetallicRoughness.metallicFactor;
		double				roughnessFactor = material.pbrMetallicRoughness.roughnessFactor;
		std::string			metallicRoughnessTexture = textureDict[material.pbrMetallicRoughness.metallicRoughnessTexture.index];
		std::vector<double> emissiveFactor = material.emissiveFactor;
		std::string			emissiveTexture = textureDict[material.emissiveTexture.index];
		std::string			normalTexture = textureDict[material.normalTexture.index];
		std::string			occlusionTexture = textureDict[material.occlusionTexture.index];

		j["meshes"][meshName]["baseColorFactor"] = baseColorFactor;
		j["meshes"][meshName]["baseColorTexture"] = baseColorTexture;
		j["meshes"][meshName]["metallicFactor"] = metallicFactor;
		j["meshes"][meshName]["roughnessFactor"] = roughnessFactor;
		j["meshes"][meshName]["metallicRoughnessTexture"] = metallicRoughnessTexture;
		j["meshes"][meshName]["emissiveFactor"] = emissiveFactor;
		j["meshes"][meshName]["emissiveTexture"] = emissiveTexture;
		j["meshes"][meshName]["normalTexture"] = normalTexture;
		j["meshes"][meshName]["occlusionTexture"] = occlusionTexture;

		// transform
		glm::mat4 transform = glm::mat4(1.0f);

		auto it = nameToIndex.find(meshName);
		if (it != nameToIndex.end())
		{
			transform = parentTransforms[it->second];
		}

		std::vector<float> transformVec = { transform[0][0],  transform[1][0], transform[2][0], transform[3][0],
												transform[0][1], transform[1][1], transform[2][1], transform[3][1],
												transform[0][2], transform[1][2], transform[2][2], transform[3][2],
												transform[0][3], transform[1][3], transform[2][3], transform[3][3] };

		j["meshes"][meshName]["transform"] = transformVec;
	}

	/*
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
		bool first = true;
		for (int& jointIndex : skin.joints)
		{
			if (first)
			{
				j["joints"][indexDict[jointIndex]]["parentID"] = -1;
				first = false;
			}
			for (int& childIndex : model.nodes[jointIndex].children)
			{
				int parentID = indexDict[jointIndex];
				j["joints"][indexDict[childIndex]]["parentID"] = parentID;
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
					// stupid fix for stupid gltf format
					j["animations"][anim.name][std::to_string(times[i])][indexDict[channel.target_node]]["translation"] = { 1.0f, 1.0f, 1.0f };
					j["animations"][anim.name][std::to_string(times[i])][indexDict[channel.target_node]]["rotation"] = { 1.0f, 1.0f, 1.0f, 1.0f };
					j["animations"][anim.name][std::to_string(times[i])][indexDict[channel.target_node]]["scale"] = { 1.0f, 1.0f, 1.0f };

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
	*/

	// WRITE to file - importing done
	std::string modelDestPath = ProjectManager::ProjectPath + ProjectManager::DefaultModelPath;
	std::filesystem::create_directories(modelDestPath);
	modelDestPath += name + ".GEM";

	std::ofstream fileOut = std::ofstream(modelDestPath, std::ios::out | std::ios::binary);
	std::vector<unsigned char> dataVec = json::to_bson(j);
	fileOut.write(reinterpret_cast<const char*>(dataVec.data()), dataVec.size());
	fileOut.close();
}

std::string Importer::getNewName(const std::string& name, const json& j, const int& count)
{
	std::string newName = name + std::to_string(count);
	if (j.count(newName) == 0)
	{
		return newName;
	}
	return getNewName(name, j, count + 1);
}

glm::mat4 Importer::getNodeTransform(const tinygltf::Node& node)
{
	if (node.matrix.size() != 0)
	{
		return glm::mat4(node.matrix[0], node.matrix[1], node.matrix[2], node.matrix[3],
						node.matrix[4], node.matrix[5], node.matrix[6], node.matrix[7], 
						node.matrix[8], node.matrix[9], node.matrix[10], node.matrix[11], 
						node.matrix[12], node.matrix[13], node.matrix[14], node.matrix[15]);
	}
	else if (node.translation.size() != 0)
	{
		glm::vec3 translation = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
		glm::quat rotation = glm::quat(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
		glm::vec3 scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);

		glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), translation) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);

		return localTransform;
	}
	else
	{
		return glm::mat4(1.0f);
	}
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

