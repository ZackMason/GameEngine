#pragma once
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"

#include <functional>
#include <string>

class Actor
{
public:
	Actor();
	Actor(std::string filename);
	Actor(std::string fn1, std::string fn2);
	Actor(const std::string& fn1, const std::string& fn2, const std::string& fn3);
	Actor(std::string fn1, std::string fn2, std::string fn3, std::string fn4);
	Actor(std::string fn1, std::string fn2, std::shared_ptr<Texture> tex);
	Actor(std::shared_ptr<Mesh> mesh, std::string fn2, std::shared_ptr<Texture> tex);
	Actor(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat);

	void Draw(Camera &camera, double time_passed);
	void Draw(Camera &camera, double time_passed, const std::vector<Light*>& light);
	
	template <typename fn>
	void Draw(Camera &camera, double time_passed, fn lambda)
	{
		glm::vec3 ray = m_transform->GetPos() - camera.GetPos();
		ray = glm::normalize(ray);
		ray *= -1.0;
		//if (glm::dot(ray, camera.GetForward()) < -0.2) { return; }

		m_material->Update(*m_transform, camera, time_passed);
		lambda();

		m_mesh->V_Draw();
	}


	Actor(const Actor& other) : Actor(other.m_mesh, other.m_material)
	{
		m_transform = other.m_transform;
	}

#if 0
	Actor& operator=(const Actor& other) 
	{
	}
#endif

	virtual ~Actor();

	std::shared_ptr<Transform> m_transform;
private:
	std::shared_ptr<Mesh>      m_mesh;
	std::shared_ptr<Material>  m_material;
	
};

