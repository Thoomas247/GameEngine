#pragma once

class UniformBuffer
{
private:
	std::array<VulkanBuffer, MAX_FRAMES_IN_FLIGHT> m_Buffers;

public:
	UniformBuffer(const uint64_t& size);

	void SetData(void* data, const uint32_t& frameIndex);
};
