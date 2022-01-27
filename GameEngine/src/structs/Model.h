#pragma once

#include "../renderer/Skeleton.h"
#include "../renderer/Mesh.h"

struct Model
{
	std::map<std::string, Mesh> m_Meshes;
	Skeleton m_Skeleton;

	Model()
	{
	}

	Model(const std::map<std::string, Mesh>& meshes, const Skeleton& skeleton)
	{
		m_Meshes = meshes;
		m_Skeleton = skeleton;
	}
};