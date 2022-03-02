#include "precompiled.h"
#include "MeshComponent.h"


MeshComponent::MeshComponent(Entity* entity, const MeshInfo& meshInfo, const MaterialShader& shader)
	: Component(entity), Pipeline(shader, meshInfo)
{
	// keep track of these to change their settings
	MeshData = meshInfo;
	Shader = shader;
}

MeshComponent::~MeshComponent()
{
	MeshData.Cleanup();
}
