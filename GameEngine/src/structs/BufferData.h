#pragma once

/*
	Contains the buffer data needed to draw meshes
*/

struct BufferData
{
	unsigned int VAO = 0;
	unsigned int NumElements = 0;

	BufferData()
	{
	}

	BufferData(const unsigned int& vao, const unsigned int& numElements)
	{
		VAO = vao;
		NumElements = numElements;
	}
};