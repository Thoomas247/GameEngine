#include "precompiled.h"
#include "Importer.h"

#include "GLTFImporter.h"

void Importer::Import(const std::string& absolutePath)
{
	auto dotPos = absolutePath.find_last_of(".");
	auto slashPos = absolutePath.find_last_of("/");
	if (dotPos == absolutePath.npos || slashPos == absolutePath.npos || dotPos < slashPos)
	{
		LOG_ERROR("IMPORTER::Invalid file path!");
	}
	slashPos++;

	std::string fileType = absolutePath.substr(dotPos + 1);
	if (std::strcmp(fileType.c_str(), "gltf") == 0)
	{
		GLTFImporter::Import(absolutePath, slashPos, dotPos, false);
	}
	else if (std::strcmp(fileType.c_str(), "glb") == 0)
	{
		GLTFImporter::Import(absolutePath, slashPos, dotPos, true);
	}
	else
	{
		LOG_ERROR("IMPORTER::Invalid file format!");
	}
}
