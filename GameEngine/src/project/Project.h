#pragma once

class Project
{
private:
	std::string m_Name;
	std::string m_ProjectDir;

	std::string m_DefaultScenesPath = "assets/scenes/";
	std::string m_DefaultTexturesPath = "assets/textures/";
	std::string m_DefaultShadersPath = "assets/shaders/";
	std::string m_DefaultCachePath = "assets/cache/";

public:
	Project(const std::string& name, const std::string& projectDir);

	std::string GetProjectDir() { return std::filesystem::absolute(m_ProjectDir).string(); }
	std::string GetDefaultScenesPath() { return m_DefaultScenesPath; }
	std::string GetDefaultTexturesPath() { return m_DefaultTexturesPath; }
	std::string GetDefaultShadersPath() { return m_DefaultShadersPath; }
	std::string GetDefaultCachePath() { return m_DefaultCachePath; }
};