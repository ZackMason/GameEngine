#pragma once
#include "../Component.h"
#include "../Entity.h"

class SphereCollider :
	public Component
{
public:
	SphereCollider(unsigned int owner) : m_owner(owner) {};
	SphereCollider() = delete;

	~SphereCollider();

private:
	unsigned int m_owner;
	float m_radius;

};

