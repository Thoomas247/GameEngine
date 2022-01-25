#include "VertexArray.h"

#include "glad/gl.h"

VertexArray::VertexArray(const std::vector<Vertex> vertices, const std::vector<unsigned int>& indices)
{
	unsigned int VBO;
	unsigned int EBO;

	glCreateVertexArrays(1, &ID);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glBindVertexArray(ID);

	glNamedBufferData(VBO, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glNamedBufferData(EBO, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(ID, 0);	// vertex positions attrib
	glVertexAttribBinding(0, 0);
	glVertexArrayAttribFormat(ID, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(ID, 1);	// vertex normals attrib
	glVertexAttribBinding(1, 0);
	glVertexArrayAttribFormat(ID, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal));

	glEnableVertexArrayAttrib(ID, 2);	// vertex texcoords attrib
	glVertexAttribBinding(2, 0);
	glVertexArrayAttribFormat(ID, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexCoord));

	glVertexArrayVertexBuffer(ID, 0, VBO, 0, sizeof(Vertex));	// bind VBO to VAO
	glVertexArrayElementBuffer(ID, EBO);	// bind EBO to VAO

	NumIndices = indices.size();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &ID);
}

void VertexArray::Bind()
{
	glBindVertexArray(ID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}
