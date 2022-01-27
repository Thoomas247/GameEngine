#pragma once

#include <string>
#include <memory>

#include "../core/Project.h"

class ProjectManager
{
private:
	static std::unique_ptr<Project> s_CurrentProject;

public:
	static void CreateProject(const std::string& name, const std::string& directory);
	//static void LoadProject();
	//static void SaveProject();

	static Project* GetCurrentProject() { return s_CurrentProject.get(); }

	static std::string GetProjectDir() { return s_CurrentProject->ProjectDir; }
	static std::string GetModelsPath() { return s_CurrentProject->ProjectDir + s_CurrentProject->DefaultModelsPath; }
	static std::string GetTexturesPath() { return s_CurrentProject->ProjectDir + s_CurrentProject->DefaultTexturesPath; }
	static std::string GetShadersPath() { return s_CurrentProject->ProjectDir + s_CurrentProject->DefaultShadersPath; }
};