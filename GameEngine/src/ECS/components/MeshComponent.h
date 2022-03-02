#pragma once
#include "Component.h"

#include "../../graphics/RenderPipeline.h"
#include "../../graphics/shaders/MaterialShader.h"

class MeshComponent : public Component
{
public:
	RenderPipeline Pipeline;
	MeshInfo MeshData;
	MaterialShader Shader;

public:
	MeshComponent(Entity* entity, const MeshInfo& meshInfo = MeshInfo(), const MaterialShader& shader = MaterialShader());
	~MeshComponent();
};
