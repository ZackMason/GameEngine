#pragma once

#include <vector>
#include <unordered_map>

#include "Component.h"
#include "Entity.h"
#include "System.h"



class EntityManager
{
	using Entities = std::unordered_map<EntityID, Entity*>;
	using Systems = std::vector<System*>;
	using Components = std::vector<std::vector<BaseComponent*>>;

	Systems m_systems;

	Entities m_entities;
public:
	Components m_components;

	EntityManager();
	~EntityManager();

	EntityManager(const EntityManager&)  = delete;
	EntityManager(const EntityManager&&) = delete;
	EntityManager& operator=(const EntityManager&)  = delete;
	EntityManager& operator=(const EntityManager&&) = delete;

	void Update(double delta);
	
	// todo use the create function // need to rewrite???
	void AddComponents(Entity* entity)
	{
		auto& comps = entity->GetComponents();
		for (auto& c : comps)
		{
			if (m_components.size() <= c.first)
				m_components.push_back(std::vector<BaseComponent*>());
			m_components[c.first].push_back(c.second);
		}
	}

	EntityID CreateEntity();
	void AddEntity(Entity* rhs) { m_entities.insert(std::make_pair(rhs->GetID(), rhs)); AddComponents(rhs); }
	void AddSystem(System* rhs) { m_systems.push_back(rhs); }
	void CreateSystem();
	void DestroyEntity(EntityID id);
};

