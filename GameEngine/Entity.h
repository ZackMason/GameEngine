#pragma once

#include <vector>
#include <map>

#include "Component.h"

typedef unsigned int EntityID;
typedef unsigned int ComponentID;

class Entity
{
	using ComponentMap = std::map<ComponentID, BaseComponent*>;

public:
	Entity(EntityID id) : m_id(id) {};
	~Entity() = default;

	Entity(const Entity&) = delete;
	Entity(Entity&&) = default;
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(Entity&&) = default;


	const EntityID GetID() const { return m_id; }
	bool TestID(EntityID rhs) const { return m_id == rhs; }

	const ComponentMap& GetComponents() const { return m_components; }

	void AddComponent(ComponentID compId, BaseComponent *pComp)
	{
		if (m_components.find(compId) == m_components.end())
			m_components.emplace(compId, pComp);
	}

	template <class CompType>
	CompType* GetComponent() const
	{
		std::cout << CompType::GetID();
		auto find = m_components.find(CompType::GetID());
		if (find != m_components.end())
			return static_cast<CompType*>(find->second);
		return nullptr;
	}

private:

	ComponentMap m_components;
	unsigned int m_id;
};

