#include "ProjectManager.h"

std::unique_ptr<Project> ProjectManager::g_CurrentProject = nullptr;

void ProjectManager::CreateProject(const std::string& name, const std::string& directory)
{
	g_CurrentProject = std::make_unique<Project>(name, directory);
}

const std::string ProjectManager::GetModelsPath()
{
    return g_CurrentProject->ProjectDir + g_CurrentProject->DefaultModelsPath;
}

const std::string ProjectManager::GetTexturesPath()
{
    return g_CurrentProject->ProjectDir + g_CurrentProject->DefaultTexturesPath;
}

const std::string ProjectManager::GetShadersPath()
{
    return g_CurrentProject->ProjectDir + g_CurrentProject->DefaultShadersPath;
}
