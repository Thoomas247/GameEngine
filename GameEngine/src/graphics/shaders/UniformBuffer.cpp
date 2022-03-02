#include "precompiled.h"
#include "UniformBuffer.h"


/* -- PUBLIC -- */

UniformBuffer::UniformBuffer(const std::string& name, const int& binding, const std::vector<Uniform>& uniforms)
{
	m_Name = name;
	m_Binding = binding;
	m_Uniforms = uniforms;
	m_Size = uniforms.size();

	//glCreateBuffers(1, &m_GLID);
	//glNamedBufferData(m_GLID, m_Size, NULL, GL_STATIC_DRAW);

	openglUpdateData();
}

void UniformBuffer::SetData(const std::string& uniformName, const std::vector<char>& data)
{
	for (Uniform& uniform : m_Uniforms)
	{
		if (uniform.Name == uniformName)
		{
			if (data.size() != uniform.Size)
			{
				LOG_ERROR("UNIFORM_BUFFER::Passed data for uniform " + uniformName + " is not the right size!");
			}
			else
			{
				uniform.Data = data;
				break;
			}
		}
	}

	openglUpdateData();
}

void UniformBuffer::Delete()
{
	//glDeleteBuffers(1, &m_GLID);
}

void UniformBuffer::Bind()
{
	//glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, m_GLID);
}


/* -- PRIVATE -- */

void UniformBuffer::openglUpdateData()
{
	for (Uniform& uniform : m_Uniforms)
	{
		//glNamedBufferSubData(m_GLID, uniform.Offset, uniform.Size, &uniform.Data[0]);
	}
}