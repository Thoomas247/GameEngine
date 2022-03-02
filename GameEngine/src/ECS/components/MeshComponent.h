#pragma once
#include "Component.h"

#include "../../graphics/RenderPipeline.h"
#include "../../graphics/shaders/MaterialShader.h"


class MeshComponent : public Component
{
public:
	RenderPipeline Pipeline;

private:
	MeshInfo m_MeshInfo;
	MaterialShader m_MaterialShader;

public:
	MeshComponent(Entity* entity, const MeshInfo& meshInfo = MeshInfo(), const MaterialShader& shader = MaterialShader());

	MeshInfo& GetMeshInfo() { return m_MeshInfo; }
	MaterialShader& GetMaterialShader() { return m_MaterialShader; }
};
