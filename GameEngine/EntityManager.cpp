#include "EntityManager.h"



EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
	for (int i = 0; i < m_components.size(); ++i)
	{
		for (auto comp : m_components[i])
		{
			size_t type_size = BaseComponent::GetTypeSize(i);
			ComponentFreeFunction free_fn = BaseComponent::GetTypeFreeFunction(i);
			free_fn(comp);
		}
	}

	for (size_t i = 0; i < m_entities.size(); i++)
	{
		delete m_entities[i];
	}
};

void EntityManager::Update(double delta)
{
}

EntityID EntityManager::CreateEntity()
{
	return EntityID();
}

void EntityManager::CreateSystem()
{
}

void EntityManager::DestroyEntity(EntityID id)
{
}
