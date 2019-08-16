#pragma once
#include "System.h"
#include "TransformComponent.h"

#include <vector>

#include "include/glm/gtc/quaternion.hpp"
#include "include/glm/gtc/matrix_access.hpp"
#include "include/glm/gtc/matrix_transform.hpp"


class Entity;
class BaseComponent;

class TransformSystem :
	public System
{
public:
	TransformSystem();
	~TransformSystem();

	TransformSystem(const TransformSystem&  rhs) = delete;
	TransformSystem(const TransformSystem&& rhs) = delete;
	TransformSystem& operator=(const TransformSystem&  rhs) = delete;
	TransformSystem&& operator=(const TransformSystem&& rhs) = delete;

	virtual bool Init() { return true; };

public:
	void Update(double delta) {};
	void UpdateQuaternion(TransformComponent* &comp);

	inline glm::mat4 GetModel(TransformComponent* &comp) const
	{
		glm::mat4 posMatrix = glm::translate(glm::mat4(1), comp->m_position);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), comp->m_scale);

		return posMatrix * comp->m_qrot * scaleMatrix;
	}
	
private:
	const float k_gravity = 9.8f;
	const float k_linear_dampening = 0.1f;
	const float k_angular_dampening = 0.01f;
};
