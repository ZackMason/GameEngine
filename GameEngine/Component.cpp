#include "Component.h"


//ComponentID GLOBAL_COMPONENT_ID_COUNT = 0;
std::vector<std::tuple<ComponentCreateFunction,
	ComponentFreeFunction, size_t>> BaseComponent::ComponentTypes;


ComponentID BaseComponent::RegisterComponent(ComponentCreateFunction createfn,
	ComponentFreeFunction freefn,
	size_t size)
{
	//return GLOBAL_COMPONENT_ID_COUNT++;
	ComponentID id = ComponentTypes.size();
	ComponentTypes.push_back(std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>(createfn,freefn,size));
	return id;
}