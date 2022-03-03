#include "precompiled.h"
#include "MeshComponent.h"


MeshComponent::MeshComponent(Entity* entity, const MeshBuffers& meshInfo, const MaterialShader& shader)
	: Component(entity)
{
	// keep track of these to change their settings
	MeshData = meshInfo;
	Shader = shader;
	Pipeline.Create(Shader, MeshData);
}

MeshComponent::~MeshComponent()
{
	MeshData.Cleanup();
}
