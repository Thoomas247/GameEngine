#include "ProjectManager.h"

std::unique_ptr<Project> ProjectManager::s_CurrentProject = nullptr;

// PUBLIC

// TODO: Remove this
void ProjectManager::CreateProject(const std::string& name, const std::string& directory)
{
	s_CurrentProject = std::make_unique<Project>(name, directory);
}