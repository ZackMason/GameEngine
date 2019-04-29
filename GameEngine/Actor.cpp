#include "Actor.h"

Actor::Actor(std::string filename)
{
	m_mesh      = std::make_shared<Mesh>     ("./res/MESHS/" + filename + ".obj");
	m_material  = std::make_shared < Material >(filename);
	m_transform = std::make_shared < Transform>();
}

Actor::Actor(std::string fn1, std::string fn2)
{
	m_mesh      = std::make_shared<Mesh>("./res/MESHS/" + fn1 + ".obj");
	m_material  = std::make_shared<Material >(fn2);
	m_transform = std::make_shared<Transform>();
}

Actor::Actor(std::string fn1, std::string fn2, std::string fn3)
{
	m_mesh      = std::make_shared<Mesh>("./res/MESHS/" + fn1 + ".obj");
	m_material  = std::make_shared<Material>(fn2,fn3);
	m_transform = std::make_shared<Transform>();
}

Actor::Actor(std::string fn1, std::string fn2, std::shared_ptr<Texture> tex)
{
	m_mesh = std::make_shared<Mesh>("./res/MESHS/" + fn1 + ".obj");
	m_material = std::make_shared<Material>(fn2, tex);
	m_transform = std::make_shared<Transform>();
}

Actor::Actor(std::string fn1, std::string fn2, std::string fn3, std::string fn4)
{
	m_mesh      = std::make_shared<Mesh>("./res/MESHS/" + fn1 + ".obj");
	m_material  = std::make_shared<Material>(fn2, fn3 , fn4);
	m_transform = std::make_shared<Transform>();
}

Actor::Actor(std::shared_ptr<Mesh> mesh, std::string fn, std::shared_ptr<Texture> tex)
{
	m_mesh = mesh;
	m_material = std::make_shared<Material>(fn, tex);
	m_transform = std::make_shared<Transform>();
}

Actor::Actor(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, std::shared_ptr<Texture> tex)
{
	m_mesh = mesh;
	m_material = mat;
	m_transform = std::make_shared<Transform>();
}

Actor::~Actor()
{
	//delete m_mesh, m_material, m_transform;
}

void Actor::Draw(Camera &camera, double time_passed)
{
	glm::vec3 ray = m_transform->GetPos() - camera.GetPos();
	ray = glm::normalize(ray);
	ray *= -1.0;
	//if (glm::dot(ray, camera.GetForward()) < -0.2) { return; }

	m_material->Update(*m_transform, camera, time_passed);

	m_mesh->V_Draw();
}
