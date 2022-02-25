#include "precompiled.h"
#include "Uniform.h"

void Uniform::Set()
{
	switch (m_Type)
	{
	case UniformType::FLOAT:
		glUniform1f(m_Location, m_Value.x);
		break;
	case UniformType::VEC2:
		glUniform2fv(m_Location, 1, &m_Value[0]);
		break;
	case UniformType::VEC3:
		glUniform3fv(m_Location, 1, &m_Value[0]);
		break;
	case UniformType::VEC4:
		glUniform4fv(m_Location, 1, &m_Value[0]);
		break;
	case UniformType::SAMPLER2D:
		glUniform1i(m_Location, m_Value.x);
		break;
	case UniformType::SAMPLER3D:
		glUniform1i(m_Location, m_Value.x);
		break;
	default:
		break;
	}
}
