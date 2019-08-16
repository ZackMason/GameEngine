#pragma once
class System
{
public:
	System() = default;
	~System() = default;
	virtual bool Init() = 0;
	virtual void Update(double delta) = 0;
};

