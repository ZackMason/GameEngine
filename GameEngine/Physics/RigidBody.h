#pragma once
#include "../Component.h"
class RigidBody :
	public Component<RigidBody>
{
public:
	RigidBody();
	~RigidBody();
};

