#include "precompiled.h"
#include "MeshComponent.h"

MeshComponent::MeshComponent(Entity* entity, const VertexArrayAsset& vertexArray, const ShaderAsset& shader, const Material& material)
	: Component(entity)
{
	m_VertexArray = vertexArray;
	m_Shader = shader;
	m_Material = material;
}