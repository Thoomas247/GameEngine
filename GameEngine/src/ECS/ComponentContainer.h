#pragma once

#include <vector>

#include "EntityModifier.h"
#include "../core/Log.h"

template <class T>
class ComponentContainer 
{
private:
	static std::vector<T> s_Components;

public:
	template <typename... P>
	static int AddComponent(P... params)
	{
		s_Components.emplace_back(params...);
		return (int)s_Components.size() - 1;
	}

	static EntityModifier RemoveComponent(const int& index)
	{
		if (index < 0 || index >= s_Components.size())
		{
			LOG_ERROR("ECS::Transform component with ID " + std::to_string(index) + " doesn't exist!");
		}

		else if (index == s_Components.size() - 1)
		{
			s_Components.erase(s_Components.end() - 1);
			return EntityModifier();
		}

		else
		{
			s_Components[index] = s_Components.back();	// replace component to delete by last component
			s_Components.erase(s_Components.end() - 1);	// delete last component

			return EntityModifier(s_Components[index].GetEntity(), index);
		}
	}

	static T& GetComponent(const int& index)
	{
		if (index >= 0 && index < s_Components.size())
		{
			return s_Components[index];
		}

		LOG_ERROR("ECS::Failed to remove transform component with ID " + std::to_string(index) + ". It doesn't exist!");
	}

	static std::vector<T>& GetAllComponents()
	{
		return s_Components;
	}

	static int GetNumComponents()
	{
		return (int)s_Components.size();
	}

};

template <class T>
std::vector<T> ComponentContainer<T>::s_Components;