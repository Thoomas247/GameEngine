#include "precompiled.h"
#include "MatrixUtil.h"

/// <summary>
/// Convert a vector of 16 floats into a glm::mat4.
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
glm::mat4 MatrixUtil::ToMat4(const std::vector<float>& vector)
{
	glm::mat4 matrix;
	matrix[0] = glm::vec4(vector[0], vector[1], vector[2], vector[3]);
	matrix[1] = glm::vec4(vector[4], vector[5], vector[6], vector[7]);
	matrix[2] = glm::vec4(vector[8], vector[9], vector[10], vector[11]);
	matrix[3] = glm::vec4(vector[12], vector[13], vector[14], vector[15]);
	return matrix;
}

/// <summary>
/// Convert a vector of 16 doubles into a glm::mat4.
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
glm::mat4 MatrixUtil::ToMat4(const std::vector<double>& vector)
{
	glm::mat4 matrix;
	matrix[0] = glm::vec4(vector[0], vector[1], vector[2], vector[3]);
	matrix[1] = glm::vec4(vector[4], vector[5], vector[6], vector[7]);
	matrix[2] = glm::vec4(vector[8], vector[9], vector[10], vector[11]);
	matrix[3] = glm::vec4(vector[12], vector[13], vector[14], vector[15]);
	return matrix;
}

/// <summary>
/// Convert a glm::mat4 into a vector for serialization.
/// </summary>
/// <param name="matrix"></param>
/// <returns></returns>
std::vector<float> MatrixUtil::FromMat4(const glm::mat4& matrix)
{
	return std::vector<float>{	matrix[0].x, matrix[0].y, matrix[0].z, matrix[0].w,
		matrix[1].x, matrix[1].y, matrix[1].z, matrix[1].w,
		matrix[2].x, matrix[2].y, matrix[2].z, matrix[2].w,
		matrix[3].x, matrix[3].y, matrix[3].z, matrix[3].w, };
}