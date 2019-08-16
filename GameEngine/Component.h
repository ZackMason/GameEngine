#pragma once

#include "ECSTypes.h"

#include <vector>
#include <tuple>
#include <limits>

class BaseComponent;

typedef size_t(*ComponentCreateFunction)(std::vector<BaseComponent*>& mem, EntityID entity, BaseComponent* comp);
typedef void(*ComponentFreeFunction)(BaseComponent* comp);

class BaseComponent
{
	static constexpr size_t INVALID_POOL_INDEX = std::numeric_limits<size_t>::max();

	static std::vector<std::tuple<ComponentCreateFunction, 
		ComponentFreeFunction, size_t>> ComponentTypes;

	size_t m_pool_index;

	EntityID m_entity_id;
public:
	BaseComponent() : m_entity_id(INVALID_ENTITY_ID), m_pool_index(INVALID_POOL_INDEX) {};
	~BaseComponent() = default;
	virtual bool Init(void* pDescription) = 0; 

	void SetEntityId(EntityID id) { m_entity_id = id; }
	void SetPoolIndex(size_t index) { m_pool_index = index; }

	EntityID GetEntityId() const { return m_entity_id; }
	size_t GetPoolIndex() const { return m_pool_index; }

	static ComponentID RegisterComponent(ComponentCreateFunction createfn, ComponentFreeFunction freefn, size_t size);

	inline static ComponentCreateFunction
	GetTypeCreateFunction
	(ComponentID id)
	{
		return std::get<0>(ComponentTypes[id]);
	}

	inline static ComponentFreeFunction
	GetTypeFreeFunction
	(ComponentID id)
	{
		return std::get<1>(ComponentTypes[id]);
	}

	inline static size_t
	GetTypeSize
	(ComponentID id)
	{
		return std::get<2>(ComponentTypes[id]);
	}
};

template <typename T>
class Component : public BaseComponent
{
public:
	static const ComponentID ID;
	static const size_t SIZE;

	static ComponentID GetID() { return ID; }

	static const ComponentCreateFunction CREATE_FUNCTION;
	static const ComponentFreeFunction FREE_FUNCTION;

};

template<typename C>
size_t ComponentCreate(std::vector<BaseComponent*>& mem, EntityID entity, BaseComponent* comp)
{
	size_t index = mem.size();
	mem.resize(index + C::SIZE);
	C* component = new(&mem[index]) C(*(C*)comp);
	component->SetEntityId(entity);
	return index;
}

template<typename C>
void ComponentFree(BaseComponent* comp)
{
	C* component = (C*)comp;
	component->~C();
}

template<typename T>
const ComponentID Component<T>::ID(BaseComponent::RegisterComponent(ComponentCreate<T>, ComponentFree<T>, sizeof(T)));

template<typename T>
const size_t Component<T>::SIZE(sizeof(T));

template<typename T>
const ComponentCreateFunction Component<T>::CREATE_FUNCTION(ComponentCreate<T>);

template<typename T>
const ComponentFreeFunction Component<T>::FREE_FUNCTION(ComponentFree<T>);



