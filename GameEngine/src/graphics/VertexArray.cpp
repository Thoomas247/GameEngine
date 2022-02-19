#include "precompiled.h"
#include "VertexArray.h"

/* -- PUBLIC -- */

void VertexArray::Delete()
{
	glDeleteVertexArrays(1, &m_GLID);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_GLID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}