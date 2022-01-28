#include "FileBrowser.h"

#include "../managers/ProjectManager.h"
#include "../managers/TextureManager.h"

constexpr auto ICON_PADDING = 24;
constexpr auto ICON_SIZE = 128 + ICON_PADDING;

// PUBLIC
FileBrowser::FileBrowser()
{
	m_Icons[DIRECTORY] = TextureManager::LoadTexture("assets/gui/icons/Folder Icon.png");
	m_Icons[GLTF] = TextureManager::LoadTexture("assets/gui/icons/File Icon.png");
	m_Icons[GEM] = TextureManager::LoadTexture("assets/gui/icons/Model Icon.png");
	m_Icons[PNG] = TextureManager::LoadTexture("assets/gui/icons/File Icon.png");
	m_Icons[OTHER] = TextureManager::LoadTexture("assets/gui/icons/File Icon.png");

	cleanAndSetPath(ProjectManager::GetProjectDir());
	getFiles();
}

void FileBrowser::Update(const float& deltaTime)
{
	ImGui::Begin("File Browser", (bool*)0, s_WindowFlags);
	ImVec2 size = ImGui::GetContentRegionAvail();

	bool shouldUpdate = false;

	if (ImGui::SmallButton("Back"))
	{
		size_t prefSeparatorIndex = m_CurrentDir.find_last_of("/");
		size_t otherSeparatorIndex = m_CurrentDir.find_last_of("\\");

		if (prefSeparatorIndex == m_CurrentDir.npos)
		{
			prefSeparatorIndex = 0;
		}
		if (otherSeparatorIndex == m_CurrentDir.npos)
		{
			otherSeparatorIndex = 0;
		}

		if (prefSeparatorIndex > otherSeparatorIndex)
		{
			m_CurrentDir = m_CurrentDir.substr(0, prefSeparatorIndex);
		}
		else if (prefSeparatorIndex < otherSeparatorIndex)
		{
			m_CurrentDir = m_CurrentDir.substr(0, otherSeparatorIndex);
		}
		// else do not change path

		shouldUpdate = true;
	}

	ImGui::SameLine();

	ImGui::Text(m_CurrentDir.c_str());

	ImGui::Separator();

	int numColumns = (int)(size.x / ICON_SIZE);
	if (numColumns < 1)
	{
		numColumns = 1;
	}

	ImGui::BeginTable("File Browser Layout", numColumns);

	for (const FileInfo& file : m_Files)
	{
		ImGui::TableNextColumn();

		ImGui::PushID(file.Name.c_str());
		ImGui::ImageButton(ImTextureID(m_Icons[file.Type]->ID), ImVec2(ICON_SIZE, ICON_SIZE));
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (file.Type == DIRECTORY)
			{
				cleanAndSetPath(file.Path);
				shouldUpdate = true;
			}

			if (file.Type == GEM)
			{
				// add model to scene
			}
		}
		ImGui::PopID();

		ImGui::Text(file.Name.c_str());
	}

	ImGui::EndTable();

	ImGui::End();

	if (shouldUpdate)
	{
		getFiles();
	}
}

// PRIVATE
void FileBrowser::destroy()
{

}

void FileBrowser::cleanAndSetPath(const std::string& newPath)
{
	m_CurrentDir = newPath;

	bool removeLast = false;

	for (auto& c : m_CurrentDir)
	{
		removeLast = false;
		if (c == '\\' || c == '/')
		{
			c = '/';
			removeLast = true;
		}
	}

	if (removeLast)
	{
		m_CurrentDir.resize(m_CurrentDir.size() - 1);
	}
}

void FileBrowser::getFiles()
{
	m_Files.clear();

	std::filesystem::directory_iterator iterator(m_CurrentDir);

	for (const auto& file : iterator)
	{
		FileInfo fileInfo;

		fileInfo.Path = file.path().string();
		fileInfo.Name = file.path().filename().string();

		if (file.is_directory())
		{
			fileInfo.Type = DIRECTORY;
		}
		else
		{
			const std::string& extension = file.path().extension().string();

			if (!extension.compare(".GEM"))
			{
				fileInfo.Type = GEM;
			}
			else if (!extension.compare(".gltf"))
			{
				fileInfo.Type = GLTF;
			}
			else if (!extension.compare(".png"))
			{
				fileInfo.Type = PNG;
			}
			else
			{
				fileInfo.Type = OTHER;
			}
		}

		m_Files.push_back(fileInfo);
	}
}
