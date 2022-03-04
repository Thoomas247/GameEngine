#include "precompiled.h"
#include "MaterialShader.h"


/* -- PUBLIC -- */

MaterialShader::MaterialShader(const std::string& glslPath)
	: Pipeline(glslPath)
{
	m_GlslPath = glslPath;
}