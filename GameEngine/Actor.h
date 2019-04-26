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
	Actor(std::string fn1, std::string fn2, std::shared_ptr<Texture> tex);
	Actor(std::shared_ptr<Mesh> mesh, std::string fn2, std::shared_ptr<Texture> tex);

	void Draw(const Camera &camera, double time_passed);

#if 0
	Actor(const Actor& other)
	{

	}

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

