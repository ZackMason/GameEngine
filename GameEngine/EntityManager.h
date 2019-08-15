#pragma once

#include <vector>
#include <map>

#include "Component.h"
#include "Entity.h"
#include "System.h"



class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	std::vector<System*> m_systems;
	std::map<unsigned int, Entity*> m_entities;
	//std::vector<Component*> m_components;
};

