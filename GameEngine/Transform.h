#pragma once

#include "include/glm/glm.hpp"
//#define GLM_ENABLE_EXPERIMENTAL
#include "include/glm/gtc/quaternion.hpp"
#include "include/glm/gtc/matrix_access.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

class Transform
{
public:
	Transform(const glm::vec3& pos = glm::vec3(0.f), const glm::vec3& rot = glm::vec3(0.f), const glm::vec3& scale = glm::vec3(1.0f,1.0f,1.0f)) :
		m_pos(pos),
		m_rot(rot),
		m_scale(scale) {
		QUpdate();
	}

	void QUpdate()
	{
		glm::quat qPitch = glm::angleAxis(m_rot.x, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(m_rot.y, glm::vec3(0, 1, 0));
		glm::quat qRoll = glm::angleAxis(m_rot.z, glm::vec3(0, 0, -1));
		glm::quat orient = qPitch * qYaw * qRoll;

		orient = glm::normalize(orient);
		m_qrot = glm::mat4_cast(orient);
		m_up = glm::vec3(glm::row(m_qrot, 1));
		m_right = glm::vec3(glm::row(m_qrot, 0));
		m_forward = glm::vec3(glm::row(m_qrot, 2));

		m_rot.z *= 0.99;
	}

	inline glm::mat4 GetModel() const
	{
		glm::mat4 posMatrix   = glm::translate(glm::mat4(1.f),m_pos);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), m_scale);

		//switch to quaternions
#if 0
		glm::mat4 rotXMatrix = glm::rotate(glm::mat4(1.f), m_rot.x, glm::vec3(1, 0, 0));
		glm::mat4 rotYMatrix = glm::rotate(glm::mat4(1.f), m_rot.y, glm::vec3(0, 1, 0));
		glm::mat4 rotZMatrix = glm::rotate(glm::mat4(1.f), m_rot.z, glm::vec3(0, 0,-1));

		//x then y then z
		glm::mat4 rotMatrix  = rotZMatrix * rotYMatrix * rotXMatrix;
#endif
		return posMatrix * m_qrot * scaleMatrix;
	}

	inline glm::vec3& GetPos  () { return m_pos;   }
	inline glm::vec3& GetRot  () { return m_rot;   }
	inline glm::vec3& GetScale() { return m_scale; }

	inline void SetPos  (const glm::vec3&   pos) { m_pos   = pos;   }
	inline void SetRotX(const float  rot) { m_rot.x = rot; QUpdate(); }
	inline void SetRotY(const float  rot) { m_rot.y = rot; QUpdate(); }
	inline void SetRotZ(const float  rot) { m_rot.z = rot; QUpdate(); }
	inline void SetRot  (const glm::vec3&   rot) { m_rot   = rot; QUpdate(); }
	inline void SetScale(const glm::vec3& scale) { m_scale = scale; }

private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
	glm::vec3 m_up;
	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::vec3 m_position;
	glm::vec3 m_dpos;
	glm::quat m_quat;
	glm::mat4 m_qrot;
};

