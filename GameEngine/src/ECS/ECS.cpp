#include "ECS.h"

#include "../core/Log.h"

std::vector<TransformComponent> ECS::s_TransformComponents;
std::vector<MeshComponent> ECS::s_MeshComponents;
std::vector<CameraComponent> ECS::s_CameraComponents;


/* CREATE */

int ECS::CreateTransformComponent(Entity* entity, const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale)
{
	int index = (int)s_TransformComponents.size();
	s_TransformComponents.push_back(TransformComponent(entity, translation, rotation, scale));
	return index;
}

int ECS::CreateMeshComponent(Entity* entity, const VertexArray& vertexArray, const Shader& shader, const Material& material)
{
	int index = (int)s_MeshComponents.size();
	s_MeshComponents.push_back(MeshComponent(entity, vertexArray, shader, material));
	return index;
}

int ECS::CreateCameraComponent(Entity* entity, const float& fov, const float& aspectRatio, const float& nearPlane, const float& farPlane)
{
	int index = (int)s_CameraComponents.size();
	s_CameraComponents.push_back(CameraComponent(entity, fov, aspectRatio, nearPlane, farPlane));
	return index;
}


/* REMOVE */

EntityModifier ECS::RemoveTransformComponent(const int& index)
{

	if (index < 0 || index >= s_TransformComponents.size())
	{
		LOG_ERROR("ECS::Transform component with ID " + std::to_string(index) + " doesn't exist!");
	}

	else if (index == s_TransformComponents.size() - 1)
	{
		s_TransformComponents.erase(s_TransformComponents.end() - 1);
		return EntityModifier();
	}

	else
	{
		s_TransformComponents[index] = s_TransformComponents.back();	// replace component to delete by last component
		s_TransformComponents.erase(s_TransformComponents.end() - 1);	// delete last component

		return EntityModifier(s_TransformComponents[index].GetEntity(), index);
	}
}

EntityModifier ECS::RemoveMeshComponent(const int& index)
{
	if (index < 0 || index >= s_MeshComponents.size())
	{
		LOG_ERROR("ECS::Mesh component with ID " + std::to_string(index) + " doesn't exist!");
	}

	else if (index == s_MeshComponents.size() - 1)
	{
		s_MeshComponents.erase(s_MeshComponents.end() - 1);
		return EntityModifier();
	}

	else
	{
		s_MeshComponents[index] = s_MeshComponents.back();	// replace component to delete by last component
		s_MeshComponents.erase(s_MeshComponents.end() - 1);	// delete last component

		return EntityModifier(s_MeshComponents[index].GetEntity(), index);
	}
}

EntityModifier ECS::RemoveCameraComponent(const int& index)
{
	if (index < 0 || index >= s_CameraComponents.size())
	{
		LOG_ERROR("ECS::Camera component with ID " + std::to_string(index) + " doesn't exist!");
	}

	else if (index == s_CameraComponents.size() - 1)
	{
		s_CameraComponents.erase(s_CameraComponents.end() - 1);
		return EntityModifier();
	}

	else
	{
		s_CameraComponents[index] = s_CameraComponents.back();	// replace component to delete by last component
		s_CameraComponents.erase(s_CameraComponents.end() - 1);	// delete last component

		return EntityModifier(s_CameraComponents[index].GetEntity(), index);
	}
}


/* GET */

TransformComponent* ECS::GetTransformComponent(const int& index)
{
	if (index >= 0 && index < s_TransformComponents.size())
	{
		return &s_TransformComponents[index];
	}

	LOG_ERROR("ECS::Failed to remove transform component with ID " + std::to_string(index) + ". It doesn't exist!");
}

MeshComponent* ECS::GetMeshComponent(const int& index)
{
	if (index >= 0 && index < s_MeshComponents.size())
	{
		return &s_MeshComponents[index];
	}

	LOG_ERROR("ECS::Failed to remove mesh component with ID " + std::to_string(index) + ". It doesn't exist!");
}

CameraComponent* ECS::GetCameraComponent(const int& index)
{
	if (index >= 0 && index < s_CameraComponents.size())
	{
		return &s_CameraComponents[index];
	}

	LOG_ERROR("ECS::Failed to remove camera component with ID " + std::to_string(index) + ". It doesn't exist!");
}