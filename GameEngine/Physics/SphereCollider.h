#pragma once
#include "../Component.h"
#include "../ECSTypes.h"
#include "../Entity.h"

class SphereCollider :
	public Component<SphereCollider>
{
public:
	SphereCollider(EntityID owner) : m_radius(1.f)
	{
		SetEntityId(owner);
		
	};
	SphereCollider() = delete;

	~SphereCollider() = default;

	virtual bool Init(void* pDescription) { return true; }

private:
	float m_radius;

};

