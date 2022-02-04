#pragma once

#include <random>

class UUID
{
private:
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_RandomEngine;
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

public:
	static uint64_t GenerateUUID();
};