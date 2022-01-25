#include "FileBrowser.h"

#include "imgui/imgui.h"

#include "../core/ProjectManager.h"
#include "../core/Log.h"

// PUBLIC
FileBrowser::FileBrowser()
{
	m_CurrentDir = ProjectManager::GetProjectDir();
	getFiles();
}

void FileBrowser::Update(const float& deltaTime)
{
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("File Browser", (bool*)0, windowFlags);

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

	ImGui::Separator();

	for (const FileInfo& file : m_Files)
	{
		if (ImGui::Button(file.Name.c_str()))
		{
			if (file.Type == DIRECTORY)
			{
				m_CurrentDir = file.Path;
				shouldUpdate = true;
			}
		}
	}

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
		}

		m_Files.push_back(fileInfo);
	}
}
