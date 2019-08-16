#pragma once
#include "Component.h"
#include "ECSTypes.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/quaternion.hpp"
#include "include/glm/gtc/matrix_access.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

#include <string>

class TransformComponent :
	public Component<TransformComponent>
{
public:
	static constexpr ComponentID ID = 0xadcdbf;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	glm::vec3 m_up;
	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::quat m_quat;
	glm::mat4 m_qrot;

	TransformComponent() = delete;
	TransformComponent(EntityID owner) :
		m_position(0), m_rotation(0), m_scale(1),
		m_up(0, 1, 0), m_forward(0, 0, -1), m_right(1, 0, 0),
		m_quat(), m_qrot()
	{
		SetEntityId(owner);
	};
	~TransformComponent() = default;

	virtual bool Init(void* pDescription) override final
	{
		return true;
	}


};

