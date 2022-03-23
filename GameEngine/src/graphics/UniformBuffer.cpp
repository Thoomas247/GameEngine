#include "precompiled.h"
#include "UniformBuffer.h"


/* -- PUBLIC -- */

UniformBuffer::UniformBuffer(const uint64_t& size)
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		m_Buffers[i].InitHostCoherent(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, size);
	}
}

void UniformBuffer::SetData(void* data, const uint32_t& frameIndex)
{
	m_Buffers[frameIndex].SetData(data);
}
