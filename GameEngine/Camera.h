#pragma once

#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/quaternion.hpp"
#include "include/glm/gtc/matrix_access.hpp"
#include "macros.h"

class Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar);

	inline glm::vec3 GetRot() const {
		glm::quat qPitch	= glm::angleAxis(m_pitch,	glm::vec3(1, 0, 0));
		glm::quat qYaw		= glm::angleAxis(m_yaw,		glm::vec3(0, 1, 0));
		glm::quat qRoll		= glm::angleAxis(m_roll,	glm::vec3(0, 0, -1));
		glm::quat orient	= qPitch * qYaw * qRoll;

		orient = glm::normalize(orient);
		return glm::eulerAngles(orient);
	}

	inline glm::mat4 GetViewProjection() const
	{
		// translate builds id mat and adds pos vec to col 4
		// order is persp * rot * pos * scale
		// in that order
		// scale is absent for camera
		auto v = glm::lookAt(m_position,m_position + m_forward, m_up);

		//return m_perspective * m_rot * glm::translate(glm::mat4(1), m_position);
		return m_perspective * v;
		//return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
	}

	inline glm::mat4 GetView() const
	{
		return glm::lookAt(m_position, m_position + m_forward, m_up);
	}

	inline glm::mat4 GetProjection() const
	{
		return m_perspective;
	}

	void SetAspect(float aspect);

	void Update    (double);
	void PosUpdate (double time_passed);
	void QuatUpdate();

	void LookAt (glm::vec3 pos);

	void RotY (float angle) { m_yaw   += angle; }
	void RotZ (float angle) { m_roll  += angle; }
	void RotX (float angle) { m_pitch += angle; }


	inline glm::vec3 GetPos()     { return m_position; }
	inline glm::vec3 GetUp()      { return       m_up; }	
	inline glm::vec3 GetRight()   { return    m_right; }
	inline glm::vec3 GetForward() { return  m_forward; }

	void MoveUp      (float amt);
	void MoveRight   (float amt);
	void MoveForward (float amt);

	float     m_fov;
	float     m_aspect;
	float     m_zNear;
	float     m_zFar;
	glm::mat4 m_perspective;
	glm::vec3 m_up;
	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::vec3 m_position;
	glm::vec3 m_dpos;
	float     m_pitch  = 0.0f;
	float     m_yaw = 0.0f;// 3.14159f;
	float     m_roll = 0.0f;
	glm::quat m_quat;
	glm::mat4 m_rot;
};