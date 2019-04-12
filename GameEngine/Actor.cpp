#include "Actor.h"

Actor::Actor(std::string filename)
{
	m_mesh      = new Mesh     ("./res/MESHS/" + filename + ".obj");
	m_material  = new Material (filename);
	m_transform = new Transform();
}

Actor::Actor(std::string fn1, std::string fn2)
{
	m_mesh      = new Mesh     ("./res/MESHS/" + fn1 + ".obj");
	m_material  = new Material (fn2);
	m_transform = new Transform();
}

Actor::Actor(std::string fn1, std::string fn2, std::string fn3)
{
	m_mesh      = new Mesh     ("./res/MESHS/" + fn1 + ".obj");
	m_material  = new Material (fn2,fn3);
	m_transform = new Transform();
}

Actor::Actor(std::string fn1, std::string fn2, std::string fn3, std::string fn4)
{
	m_mesh      = new Mesh     ("./res/MESHS/" + fn1 + ".obj");
	m_material  = new Material (fn2, fn3 , fn4);
	m_transform = new Transform();
}

Actor::~Actor()
{
	//delete m_mesh, m_material, m_transform;
}

void Actor::Draw(const Camera &camera, double time_passed)
{
	m_material->Update(*m_transform, camera, time_passed);

	m_mesh->V_Draw();
}
