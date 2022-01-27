#pragma once

#include "Panel.h"

#include <vector>
#include <map>
#include <filesystem>

#include "../structs/Texture.h"

enum FileType {
	NONE, DIRECTORY, GLTF, GEM, PNG, OTHER
};

struct FileInfo
{
	std::string Name;
	std::string Path;
	FileType Type = NONE;
};

class FileBrowser : public Panel
{
private:
	std::vector<FileInfo> m_Files;
	std::string m_CurrentDir;
	std::map<FileType, std::shared_ptr<Texture>> m_Icons;

public:
	FileBrowser();
	void Update(const float& deltaTime) override;

private:
	void destroy() override;
	void cleanAndSetPath(const std::string& newPath);
	void getFiles();
};