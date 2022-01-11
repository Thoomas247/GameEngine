#include "ProjectManager.h"

std::unique_ptr<Project> ProjectManager::CurrentProject = nullptr;

void ProjectManager::CreateProject(const std::string& name, const std::string& directory)
{
	CurrentProject = std::make_unique<Project>(name, directory);
}

const std::string ProjectManager::GetModelsPath()
{
	return CurrentProject->ProjectDir + CurrentProject->DefaultModelsPath;
}

const std::string ProjectManager::GetTexturesPath()
{
	return CurrentProject->ProjectDir + CurrentProject->DefaultTexturesPath;
}

const std::string ProjectManager::GetShadersPath()
{
	return CurrentProject->ProjectDir + CurrentProject->DefaultShadersPath;
}