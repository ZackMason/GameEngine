#pragma once
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Shader.h"

#include <functional>
#include <string>

class Actor
{
public:
	Actor();
	Actor(const std::string& fn1) : m_material(std::make_shared<Material>(fn1)), m_transform(std::make_shared<Transform>())
	{
		m_mesh = std::make_shared<Mesh>("./res/MESHS/" + fn1 + ".obj");
		//m_material->AddSdr(std::make_shared<Shader>(fn1));
	}

	Actor(const std::string& fn1, const std::string& fn2) : m_material(std::make_shared<Material>(fn2)), m_transform(std::make_shared<Transform>())
	{
		m_mesh = std::make_shared<Mesh>("./res/MESHS/" + fn1 + ".obj");
		//m_material->AddSdr(std::make_shared<Shader>("./res/SHADERS/" + fn2 + "Shader"));
	}

	Actor(const std::string& fn1, const std::string& fn2, const std::string& fn3) : m_material(std::make_shared<Material>(fn2,fn3)), m_transform(std::make_shared<Transform>())
	{
		m_mesh = std::make_shared<Mesh>("./res/MESHS/" + fn1 + ".obj");
		//std::shared_ptr<Shader> sdr = std::make_shared<Shader>("./res/SHADERS/" + fn2 + "Shader");
		//std::shared_ptr<Texture> tex = std::make_shared<Texture>("./res/TEXTURES/" + fn3 + ".jpg");
		//m_material->Add(tex);
		//m_material->AddSdr(sdr);
	}

	Actor(const std::string& fn1, const std::string& fn2, const std::shared_ptr<Texture>&  tex) : m_material(std::make_shared<Material>(fn2)), m_transform(std::make_shared<Transform>())
	{
		m_mesh = std::make_shared<Mesh>("./res/MESHS/" + fn1 + ".obj");
		m_material->Add(tex);
		//m_material->AddSdr(std::make_shared<Shader>("./res/SHADERS/" + fn2 + "Shader"));
	}

	Actor(const std::shared_ptr<Mesh>& mesh, const std::string& fn1, const std::shared_ptr<Texture>& tex) : m_material(std::make_shared<Material>(fn1)), m_transform(std::make_shared<Transform>())
	{
		m_mesh = mesh;
		m_material->Add(tex);
	//	m_material->AddSdr(std::make_shared<Shader>("./res/SHADERS/" + fn1 + "Shader"));
	}

	Actor(const std::shared_ptr<Mesh> & mesh, const std::shared_ptr<Material>& mat) : m_mesh(mesh) , m_material(mat), m_transform(std::make_shared<Transform>())
	{
	}

	void Draw(Camera &camera, float time_passed, const std::vector<Light*>& light = std::vector<Light*>())
	{
		glm::vec3 ray = m_transform->GetPos() - ((-1.f*camera.GetPos()) + (camera.GetForward()*10.f));
		ray = glm::normalize(ray);
		ray *= -1.0;
		if (glm::dot(ray, camera.GetForward()) < -0.42) { return; }

		m_material->Bind();
		m_material->Get_Sdr()->SetMVP(*m_transform, camera);
		m_material->Get_Sdr()->setInt("material.diffuse", 0);
		if (m_material->GetTexSize() > 1)
		m_material->Get_Sdr()->setInt("material.specular", 1);

		//if (!light.empty())
			//m_material->Get_Sdr()->SetLights(light);
		m_material->Get_Sdr()->setFloat("Time", time_passed);

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

	std::shared_ptr<Material> Get_Mat() { return m_material; }
	std::shared_ptr<Shader>   Get_Sdr() { return m_material->Get_Sdr(); }

	virtual ~Actor() = default;

	std::shared_ptr<Transform> m_transform;
private:
	std::shared_ptr<Mesh>      m_mesh;
	std::shared_ptr<Material>  m_material;
	
};

