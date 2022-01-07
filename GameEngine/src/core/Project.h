#ifndef GAME_PROJECT
#define GAME_PROJECT

#include <string>

struct Project
{
	std::string Name;
	std::string ProjectDir;

	std::string DefaultModelsPath = "assets/models/";
	std::string DefaultTexturesPath = "assets/textures/";
	std::string DefaultShadersPath = "assets/shaders/";

	Project(const std::string& name, const std::string& projectDir)
	{
		Name = name;
		ProjectDir = projectDir;
	}
};

#endif // !GAME_PROJECT