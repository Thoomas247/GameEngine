#pragma once

#include "../project/Project.h"

class ProjectManager
{
private:
	static std::unique_ptr<Project> s_CurrentProject;

public:
	static void CreateProject(const std::string& name, const std::string& directory);

	//static void LoadProject();
	//static void SaveProject();

	static std::string GetProjectDir() { return s_CurrentProject->GetProjectDir(); }
	static std::string GetScenesPath() { return s_CurrentProject->GetProjectDir() + s_CurrentProject->GetDefaultScenesPath(); }
	static std::string GetTexturesPath() { return s_CurrentProject->GetProjectDir() + s_CurrentProject->GetDefaultTexturesPath(); }
	static std::string GetShadersPath() { return s_CurrentProject->GetProjectDir() + s_CurrentProject->GetDefaultShadersPath(); }
};