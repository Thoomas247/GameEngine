#pragma once

#include "../renderer/Skeleton.h"
#include "../renderer/Mesh.h"

struct Model
{
	std::string m_Name;
	std::vector<Mesh> m_Meshes;
	Skeleton m_Skeleton;
	bool m_HasSkeleton;

	Model()
	{
		m_HasSkeleton = false;
	}

	Model(const std::string& name, const std::vector<Mesh>& meshes, const Skeleton& skeleton, const bool& hasSkeleton = true)
	{
		m_Name = name;
		m_Meshes = meshes;
		m_Skeleton = skeleton;
		m_HasSkeleton = hasSkeleton;
	}
};