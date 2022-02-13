#pragma once

#include <vector>

#include "glm/glm.hpp"

namespace MatrixUtil
{
	glm::mat4 ToMat4(const std::vector<float>& vector);
	glm::mat4 ToMat4(const std::vector<double>& vector);
	std::vector<float> FromMat4(const glm::mat4& matrix);
}