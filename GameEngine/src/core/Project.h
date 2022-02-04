#pragma once

#include <string>

class Project
{
private:
	std::string m_Name;
	std::string m_ProjectDir;

	std::string m_DefaultScenesPath = "assets/scenes/";
	std::string m_DefaultModelsPath = "assets/models/";
	std::string m_DefaultTexturesPath = "assets/textures/";
	std::string m_DefaultShadersPath = "assets/shaders/";

public:
	Project(const std::string& name, const std::string& projectDir);
	
	std::string GetProjectDir() { return m_ProjectDir; }
	std::string GetDefaultScenesPath() { return m_DefaultScenesPath; }
	std::string GetDefaultModelsPath() { return m_DefaultModelsPath; }
	std::string GetDefaultTexturesPath() { return m_DefaultTexturesPath; }
	std::string GetDefaultShadersPath() { return m_DefaultShadersPath; }
};