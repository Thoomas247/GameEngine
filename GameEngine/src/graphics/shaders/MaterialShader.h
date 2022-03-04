#pragma once

class MaterialShader
{
public:
	VulkanPipeline Pipeline;

private:
	static const unsigned int NOT_COMPILED = (unsigned int)(-1);

	std::string m_GlslPath;

public:
	MaterialShader(const std::string& glslPath);
};