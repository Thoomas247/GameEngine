#pragma once

class RenderPipeline
{
public:
	VkPipeline Pipeline;

public:
	RenderPipeline();

	void Create(const MaterialShader& shader, const MeshBuffers& meshInfo);
};

