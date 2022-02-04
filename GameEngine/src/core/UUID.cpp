#include "UUID.h"

std::random_device UUID::s_RandomDevice;
std::mt19937_64 UUID::s_RandomEngine(s_RandomDevice());
std::uniform_int_distribution<uint64_t> UUID::s_UniformDistribution;

/// <summary>
/// Returns a randomly generated unique identifier.
/// This is not the "proper" way of generating UUIDs but should work for now.
/// </summary>
/// <returns></returns>
uint64_t UUID::GenerateUUID()
{
	return s_UniformDistribution(s_RandomEngine);
}
