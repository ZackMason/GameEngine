#pragma once

#include <vector>
#include <map>

#include "Component.h"

typedef unsigned int EntityID;
typedef unsigned int ComponentID;

class Entity
{
public:
	Entity(EntityID id) : m_id(id) {};
	~Entity() = default;


	const EntityID GetID() const { return m_id; }
	bool TestID(EntityID rhs) const { return m_id == rhs; }

private:
	std::map<ComponentID, Component*> m_components;
	unsigned int m_id;
};

