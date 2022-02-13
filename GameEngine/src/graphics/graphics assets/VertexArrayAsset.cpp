#include "VertexArrayAsset.h"

#include "glad/gl.h"

/* -- PUBLIC -- */

void VertexArrayAsset::Delete()
{
	glDeleteVertexArrays(1, &m_GLID);
}

void VertexArrayAsset::Bind()
{
	glBindVertexArray(m_GLID);
}

void VertexArrayAsset::Unbind()
{
	glBindVertexArray(0);
}