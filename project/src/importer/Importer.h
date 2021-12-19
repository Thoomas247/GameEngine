#ifndef IMPORTER
#define IMPORTER

#include <string>
#include <vector>
#include <map>

#include "tinygltf/tiny_gltf.h"


namespace Importer
{
	void ImportGLTF(const std::string& name, const std::string& path);

	std::vector<float> getVertexPositions(tinygltf::Model& model, tinygltf::Primitive& primitive);
	std::vector<float> getVertexNormals(tinygltf::Model& model, tinygltf::Primitive& primitive);
	std::vector<float> getVertexTextureCoords(tinygltf::Model& model, tinygltf::Primitive& primitive);
	std::vector<float> getVertexColors(tinygltf::Model& model, tinygltf::Primitive& primitive);
	std::vector<unsigned short> getVertexJoints(tinygltf::Model& model, tinygltf::Primitive& primitive);
	std::vector<float> getVertexWeights(tinygltf::Model& model, tinygltf::Primitive& primitive);
	std::vector<unsigned int> getIndices(tinygltf::Model& model, tinygltf::Primitive& primitive);
}

#endif // !IMPORTER