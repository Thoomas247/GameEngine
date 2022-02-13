#pragma once

#include <string>
#include <map>

#include "glm/glm.hpp"
#include "tinygltf/tiny_gltf.h"

#include "json/json.hpp"
using json = nlohmann::json;

class GLTFImporter
{
public:
	static void Import(const std::string& absolutePath);

private:
	static std::string copyTexture(const std::string& modelPath, const std::string& uri, const size_t& slashPos, const size_t& dotPos);
	static glm::mat4 getNodeTransform(const tinygltf::Node& node);
	static std::string getNewMeshName(const std::string& name, const std::map<std::string, int>& map, const int& count = 0);
	//static std::map<unsigned int, unsigned int> processJoints(tinygltf::Model& model, json& j);

	static std::vector<float> getVertexPositions(tinygltf::Model& model, tinygltf::Primitive& primitive);
	static std::vector<float> getVertexNormals(tinygltf::Model& model, tinygltf::Primitive& primitive);
	static std::vector<float> getVertexTextureCoords(tinygltf::Model& model, tinygltf::Primitive& primitive);
	static std::vector<float> getVertexColors(tinygltf::Model& model, tinygltf::Primitive& primitive);
	//static std::vector<unsigned short> getVertexJoints(tinygltf::Model& model, tinygltf::Primitive& primitive, std::map<unsigned int, unsigned int> indexDict);
	//static std::vector<float> getVertexWeights(tinygltf::Model& model, tinygltf::Primitive& primitive);

	static std::vector<unsigned int> getIndices(tinygltf::Model& model, tinygltf::Primitive& primitive);
};

