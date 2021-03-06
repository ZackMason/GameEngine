#pragma once

#include "include/glm/glm.hpp"

class Light
{
public:
	Light() : m_position(0), m_color(1.f) , m_ads(.4, 0.9, 1.0) , m_clq(.9, 0.009, 0.0000032) {};
	~Light() = default;

	virtual void Init() = 0;


	glm::vec3 m_clq; // x:constant y:linear z:quadratic
	glm::vec4 m_position;
	glm::vec4 m_color;
	glm::vec3 m_ads; // x:ambient y:diffuse z:spec
};

