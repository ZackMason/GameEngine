#pragma once
class Component
{
public:
	Component() = default;
	~Component() = default;
	virtual bool Init(void* pDescription) = 0; 
};

