#include "VertexArray.h"

#include "glad/gl.h"

// PUBLIC
VertexArray::VertexArray()
{
}

VertexArray::VertexArray(const std::vector<Vertex> vertices, const std::vector<unsigned int>& indices)
{
	unsigned int VBO;
	unsigned int EBO;

	glCreateVertexArrays(1, &m_ID);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glBindVertexArray(m_ID);

	glNamedBufferData(VBO, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glNamedBufferData(EBO, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(m_ID, 0);	// vertex positions attrib
	glVertexAttribBinding(0, 0);
	glVertexArrayAttribFormat(m_ID, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(m_ID, 1);	// vertex normals attrib
	glVertexAttribBinding(1, 0);
	glVertexArrayAttribFormat(m_ID, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal));

	glEnableVertexArrayAttrib(m_ID, 2);	// vertex texcoords attrib
	glVertexAttribBinding(2, 0);
	glVertexArrayAttribFormat(m_ID, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexCoord));

	glVertexArrayVertexBuffer(m_ID, 0, VBO, 0, sizeof(Vertex));	// bind VBO to VAO
	glVertexArrayElementBuffer(m_ID, EBO);	// bind EBO to VAO

	m_NumIndices = indices.size();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_ID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}