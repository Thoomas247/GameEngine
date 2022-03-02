#include "precompiled.h"
#include "MeshComponent.h"


MeshComponent::MeshComponent(Entity* entity, const MeshInfo& meshInfo = MeshInfo(), const MaterialShader& shader = MaterialShader())
	: Component(entity), Pipeline(shader, meshInfo)
{
	// keep track of these to change their settings
	m_MeshInfo = meshInfo;
	m_MaterialShader = shader;
}
