#pragma once

#include "Panel.h"

#include <vector>
#include <map>
#include <filesystem>

#include "../structs/Texture.h"

enum class FileType {
	None, Directory, GLTF, GEM, PNG, Other
};

struct FileInfo
{
	std::string Name;
	std::string Path;
	FileType Type = FileType::None;
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
	std::string cleanPath(std::string path);
	void cleanAndSetPath(const std::string& newPath);
	void getFiles();
};