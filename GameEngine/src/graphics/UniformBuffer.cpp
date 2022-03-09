#include "precompiled.h"
#include "UniformBuffer.h"


/* -- PUBLIC -- */

UniformBuffer::UniformBuffer(const std::string& name, const int& binding, const std::vector<Uniform>& uniforms)
{
	m_Name = name;
	m_Binding = binding;
	m_Uniforms = uniforms;
	m_Size = (int)uniforms.size();
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
}

void UniformBuffer::Delete()
{

}

void UniformBuffer::Bind()
{

}