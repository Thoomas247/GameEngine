#pragma once
#include "Component.h"

#include "../../graphics/RenderPipeline.h"
#include "../../graphics/shaders/MaterialShader.h"

class MeshComponent : public Component
{
public:
	RenderPipeline Pipeline;
	MeshBuffers MeshData;
	MaterialShader Shader;

public:
	MeshComponent(Entity* entity, const MeshBuffers& meshInfo = MeshBuffers(), const MaterialShader& shader = MaterialShader());
	~MeshComponent();
};
