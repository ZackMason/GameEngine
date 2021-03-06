#include "Camera.h"
#include <iostream>

Camera::Camera(const glm::vec3 &pos, float fov, float aspect, float zNear, float zFar)
{
    m_fov         = fov;
    m_zFar        = zFar;
    m_zNear       = zNear;
    m_aspect      = aspect;

    m_position    = pos;
    m_dpos        = glm::vec3(0, 0, 0);

    m_up          = glm::vec3(0, 1, 0);
    m_right       = glm::vec3(1, 0, 0);
    m_forward     = glm::vec3(0, 0,-1);
    m_quat        = glm::quat(glm::quatLookAt(m_forward, m_up));
    
	m_perspective = glm::perspective(fov * RAD, aspect, zNear, zFar);
}

void Camera::SetAspect(float aspect)
{
    m_aspect      = aspect;
    m_perspective = glm::perspective(m_fov * RAD, m_aspect, m_zNear, m_zFar);
}

void Camera::PosUpdate(double time_passed)
{
	if (m_dpos.length() <= 0) return;
    m_position += m_dpos * (float)time_passed;
    m_dpos     *= 0.8f;
	if (m_dpos.length() < 1.5)
		m_dpos = glm::vec3(0, 0, 0);
    //a	m_position.y > 3.0f ? m_dpos.y -= 9.851f : m_position.y = 3.10f;
	if (m_position.y < 3.0f)
		m_position.y = 3.01f;
}

void Camera::QuatUpdate()
{
    glm::quat qPitch = glm::angleAxis(m_pitch, glm::vec3(1, 0, 0));
    glm::quat qYaw   = glm::angleAxis(m_yaw  , glm::vec3(0, 1, 0));
    glm::quat qRoll  = glm::angleAxis(m_roll , glm::vec3(0, 0, -1));
    glm::quat orient = qPitch * qYaw * qRoll;

    orient    = glm::normalize(orient);
    m_rot     = glm::mat4_cast(orient);
    m_up      = glm::vec3(glm::row(m_rot, 1));
    m_right   = glm::vec3(glm::row(m_rot, 0));
    m_forward = glm::vec3(glm::row(m_rot, 2));
}

void Camera::LookAt(glm::vec3 pos)
{
	auto v = glm::normalize(pos - m_position);
	glm::quat look_quat = glm::lookAt(m_position, m_position + v, m_up);
	//look_quat = glm::normalize(look_quat);
	//m_pitch = ang.x;
	auto f = glm::yaw(look_quat) - m_yaw;
	std::cout << "delta yaw: " << f << " && x: " << v.x << " z: " << v.z <<  "\n";
	m_yaw += f;
	//m_roll = ang.z;
}

void Camera::Update(double time_passed)
{
    QuatUpdate();
    PosUpdate (time_passed);
}

void Camera::MoveForward(float amt)
{
    glm::vec3 t = m_forward;
    //t.y         = 0;
    t           = glm::normalize(t);
    m_dpos     += t * amt;
}

void Camera::MoveUp(float amt)
{
    m_dpos += glm::vec3(0, 1, 0) * amt;
}

void Camera::MoveRight(float amt)
{
    m_dpos += m_right * amt;
}
