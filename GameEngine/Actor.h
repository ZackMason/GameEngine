#pragma once
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "Material.h"

#include <string>

class Actor
{
public:
	Actor();
	Actor(std::string filename);
	Actor(std::string fn1, std::string fn2);
	Actor(std::string fn1, std::string fn2, std::string fn3);
	Actor(std::string fn1, std::string fn2, std::string fn3, std::string fn4);

	void Draw(const Camera &camera, double time_passed);

#if 0
	Actor(const Actor& other)
	{
		m_mesh = new Mesh(other.m_mesh);
		m_material = new Material(other.m_material);
	}

	Actor& operator=(const Actor& other) 
	{
		if (this == &other) { return *this; }
		this->m_mesh = new Mesh(other.m_mesh);
		this->m_material = new Material(other.m_material);
		return *this;
	}
#endif

	virtual ~Actor();

	Transform *m_transform;
private:
	Mesh      *m_mesh;
	Material  *m_material;
};

