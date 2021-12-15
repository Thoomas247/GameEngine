#ifndef IMPORTER
#define IMPORTER

#include <string>
#include <vector>


#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "tinygltf/tiny_gltf.h"

#include "glm/glm.hpp"

namespace Importer
{
	void ImportGLTF(const std::string& name, const std::string& path);

	void getVertexPositions(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<glm::vec3>& vertexPositions);
	void getVertexNormals(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<glm::vec3>& vertexNormals);
	void getVertexTextureCoords(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<glm::vec2>& vertexTextureCoords);
	void getVertexColors(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<glm::vec4>& vertexColors);
	void getVertexJoints(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<glm::ivec4>& vertexJoints);
	void getVertexWeights(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<glm::vec4>& vertexWeights);
}

#endif // !IMPORTER