#include "Project.h"

#include "../core/Log.h"

/* -- PUBLIC -- */

Project::Project(const std::string& name, const std::string& projectDir)
{
	m_Name = name;
	m_ProjectDir = projectDir;
}