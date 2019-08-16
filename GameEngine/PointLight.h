#pragma once
#include "Light.h"
class PointLight :
	public Light
{
public:
	PointLight() = default;
	~PointLight() = default;

	virtual void Init() override final
	{
		m_position = glm::vec4(0);
		m_color = glm::vec4(1.f);
		m_ads = glm::vec3(.4, 0.9, 1.0);
		m_clq = glm::vec3(.9, 0.009, 0.0000032);
	}

};

