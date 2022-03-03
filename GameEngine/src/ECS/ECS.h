#pragma once

#include <vector>

#include "ComponentContainer.h"


class ECS
{
public:

	/// <summary>
	/// Create a component and add it to the appropriate vector.
	/// </summary>
	/// <typeparam name="T">The type of component (Transform, Mesh, Camera...)</typeparam>
	/// <typeparam name="...P">Variable list of parameters to pass to constructor</typeparam>
	/// <returns>The index where the component was added</returns>
	template <class T, typename... P>
	static int CreateComponent(P... params)
	{
		return ComponentContainer<T>::AddComponent(params...);
	}

	/// <summary>
	/// Delete the component with the given type and index.
	/// </summary>
	/// <typeparam name="T">The type of component (Transform, Mesh, Camera...)</typeparam>
	/// <param name="index">The index where the component was added</param>
	/// <returns>An EntityModifier instance</returns>
	template <class T>
	static EntityModifier RemoveComponent(const int& index)
	{
		return ComponentContainer<T>::RemoveComponent(index);
	}

	/// <summary>
	/// Get the component with the given type and index.
	/// </summary>
	/// <typeparam name="T">The type of component (Transform, Mesh, Camera...)</typeparam>
	/// <param name="index">The index where the component was added</param>
	/// <returns>The component at the given index</returns>
	template <class T>
	static T& GetComponent(const int& index)
	{
		return ComponentContainer<T>::GetComponent(index);
	}

	/// <summary>
	/// Get the whole vector of components of the given type.
	/// </summary>
	/// <typeparam name="T">The type of component (Transform, Mesh, Camera...)</typeparam>
	/// <returns>A reference to a vector of components of type T</returns>
	template <class T>
	static std::vector<T>& GetComponents()
	{
		return ComponentContainer<T>::GetAllComponents();
	}

	/// <summary>
	/// Get the number of components of a given type.
	/// </summary>
	/// <typeparam name="T">The type of component (Transform, Mesh, Camera...)</typeparam>
	/// <returns>An int representing the size of the vector of components of type T</returns>
	template <class T>
	static int GetNumComponents()
	{
		return ComponentContainer<T>::GetNumComponents();
	}
};
