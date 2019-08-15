#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
public:
	DirectionalLight() = default;
	~DirectionalLight() = default;

	virtual void Init() override final 
	{
		m_position = glm::vec4(1, -1, 0, 1);
		m_color = glm::vec4(.15);
		m_ads = glm::vec3(.4, 0.9, 1.0);
		m_clq = glm::vec3(.9, 0.009, 0.0000032);
	}

};

