#pragma once

#include <map>
#include <string>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "../core/UUID.h"
#include "../graphics/GraphicsAssetManager.h"

#include "ECS.h"

constexpr int MAX_NUM_COMPONENT_TYPES = 32;


class ComponentTypeID
{
private:
	static int m_Counter;
public:
	/// <summary>
	/// Returns a unique ID for each type that is passed in.
	/// </summary>
	/// <typeparam name="T">Component type</typeparam>
	/// <returns>A unique int belonging to the given type</returns>
	template<class T>
	static int Value()
	{
		static int id = m_Counter++;
		return id;
	}
};

class Entity
{
private:
	uint64_t m_UUID;
	std::string m_Name;

	Entity* m_Parent;
	std::map<uint64_t, std::shared_ptr<Entity>> m_ChildrenEntities;

	int m_Components[MAX_NUM_COMPONENT_TYPES];

public:
	Entity()
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = "Empty Entity";

		for (int i = 0; i < MAX_NUM_COMPONENT_TYPES; i++) {
			m_Components[i] = -1;
		}
	}

	Entity(const std::string& name, Entity* parent)
	{
		m_UUID = UUID::GenerateUUID();
		m_Name = name;

		m_Parent = parent;

		for (int i = 0; i < MAX_NUM_COMPONENT_TYPES; i++) {
			m_Components[i] = -1;
		}
	}

	const uint64_t GetUUID() const { return m_UUID; }
	const std::string& GetName() const { return m_Name; }
	const std::map<uint64_t, std::shared_ptr<Entity>>& GetChildren() const { return m_ChildrenEntities; }

	Entity* GetParent();
	void SetParent(Entity* parent) { m_Parent = parent; }

	std::shared_ptr<Entity> CreateChild(const std::string& name);
	void AddChild(const std::shared_ptr<Entity>& entity);
	void RemoveChild(const uint64_t& entityID);

	/// <summary>
	/// Finds the closest transform component which is above this entity in the scene tree.
	/// </summary>
	/// <returns></returns>
	int FindTransformParent();


	/* COMPONENT FUNCTIONS */

	template <class T>
	bool HasComponent()
	{
		static int n = ComponentTypeID::Value<T>();
		return m_Components[n] != -1;
	}

	template <class T>
	void SetIndex(const int& index)
	{
		static int n = ComponentTypeID::Value<T>();
		m_Components[n] = index;
	}

	template <class T>
	int GetIndex()
	{
		static int n = ComponentTypeID::Value<T>();
		return m_Components[n];
	}

	template <class T, typename... P>
	void AddComponent(P... params)
	{
		static int n = ComponentTypeID::Value<T>();
		m_Components[n] = ECS::CreateComponent<T>(this, params...);
	}

	template<class T>
	void RemoveComponent()
	{
		static int n = ComponentTypeID::Value<T>();
		EntityModifier modifier = ECS::RemoveComponent<T>(m_Components[n]);
		m_Components[n] = -1;

		if (modifier.EntityToModify != nullptr)
		{
			modifier.EntityToModify->SetIndex<T>(modifier.NewComponentIndex);	// update the entity whose component was moved
		}
	}

	template<class T>
	T& GetComponent()
	{
		static int n = ComponentTypeID::Value<T>();

		if (m_Components[n] != -1)
		{
			return ECS::GetComponent<T>(m_Components[n]);
		}

		LOG_ERROR("ENTITY::Could not find component for entity " + std::to_string(m_UUID) + "!");
	}


private:
	/// <summary>
	/// Helper function to update this entity's children's transform's parents when this entity's transform has moved.
	/// </summary>
	/// <param name="index"></param>
	void setChildrenTransformParents(const int& index);
};
