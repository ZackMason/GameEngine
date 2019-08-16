#pragma once
#include "../System.h"
#include "../Entity.h"

#include <vector>

class PhysicsSystem :
	public System
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	PhysicsSystem(const PhysicsSystem&  rhs) = delete;
	PhysicsSystem(const PhysicsSystem&& rhs) = delete;
	PhysicsSystem& operator=(const PhysicsSystem&  rhs) = delete;
	PhysicsSystem&& operator=(const PhysicsSystem&& rhs) = delete;

public:
	void Update(const std::vector<Entity*> &entities);

private:
	const float k_gravity = 9.8f;
	const float k_linear_dampening = 0.1f;
	const float k_angular_dampening = 0.01f;
};

