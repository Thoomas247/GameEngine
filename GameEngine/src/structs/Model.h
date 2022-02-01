#pragma once

#include "../renderer/Skeleton.h"
#include "../renderer/Mesh.h"

struct Model
{
	std::map<std::string, Mesh> m_Meshes;
	Skeleton m_Skeleton;
	bool m_HasSkeleton;

	Model()
	{
		m_HasSkeleton = false;
	}

	Model(const std::map<std::string, Mesh>& meshes, const Skeleton& skeleton, const bool& hasSkeleton = true)
	{
		m_Meshes = meshes;
		m_Skeleton = skeleton;
		m_HasSkeleton = hasSkeleton;
	}
};