#pragma once

#include <string>
#include <memory>

#include "Project.h"

namespace ProjectManager
{
	extern std::unique_ptr<Project> CurrentProject;

	void CreateProject(const std::string& name, const std::string& directory);
	//void LoadProject();
	//void SaveProject();

	const std::string GetProjectDir();
	const std::string GetModelsPath();
	const std::string GetTexturesPath();
	const std::string GetShadersPath();

	/*
	void SetModelsPath(const std::string& relativePath);
	void SetTexturesPath(const std::string& relativePath);
	void SetShadersPath(const std::string& relativePath);
	*/
}