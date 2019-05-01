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

void Camera::PosUpdate()
{
    m_position += m_dpos;
    m_dpos     *= 0.8f;
    //((m_position.y < 0.0f) ? m_dpos.y += 0.051f : m_dpos.y = 0.0f);
}

void Camera::QuatUpdate()
{
    glm::quat qPitch = glm::angleAxis(m_pitch, glm::vec3(1, 0, 0));
    glm::quat qYaw   = glm::angleAxis(m_yaw  , glm::vec3(0, 1, 0));
    glm::quat qRoll  = glm::angleAxis(m_roll , glm::vec3(0, 0,-1));
    glm::quat orient = qPitch * qYaw;

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
	look_quat = glm::normalize(look_quat);
	//m_pitch = ang.x;
	std::cout << glm::yaw(look_quat) - m_yaw << "&&" << v.x << "\n";
	m_yaw += glm::yaw(look_quat) - m_yaw;
	//m_roll = ang.z;
}

void Camera::Update()
{
    QuatUpdate();
    PosUpdate ();
}

void Camera::MoveForward(float amt)
{
    glm::vec3 t = m_forward;
    t.y         = 0;
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
