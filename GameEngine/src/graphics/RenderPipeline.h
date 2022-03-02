#pragma once

class RenderPipeline
{
public:
	VkPipeline Pipeline;

public:
	RenderPipeline(const MaterialShader& shader, const MeshInfo& meshInfo);
};

