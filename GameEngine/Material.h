#pragma once

#include <string>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Transform.h"
#include "Light.h"

class Material
{
public:
	Material();

	Material(std::string filename) 
	{
		m_shader  = std::make_shared< Shader >("./res/SHADERS/"  + filename + "Shader");
		m_textures.push_back(std::make_shared<Texture>("./res/TEXTURES/" + filename + ".jpg"  ));
		if (!m_textures[0]->IsLoaded())
			m_textures.push_back(std::make_shared< Texture>("./res/TEXTURES/" + filename + ".png"));
	}

	Material(std::string fn1, std::string fn2)
	{
		m_shader  = std::make_shared<Shader>("./res/SHADERS/"  + fn1 + "Shader");
		m_textures.push_back(std::make_shared<Texture>("./res/TEXTURES/" + fn2 + ".jpg"  ));
		if (!m_textures[0]->IsLoaded())
			m_textures.push_back(std::make_shared<Texture>("./res/TEXTURES/" + fn2 + ".png"));
	}

	Material(std::string fn1, std::string fn2, std::string fn3)
	{
		m_shader  =  std::make_shared<Shader> ("./res/SHADERS/"  + fn1 + "Shader");
		m_textures.push_back(std::make_shared<Texture>("./res/TEXTURES/" + fn2 + fn3     ));
	}

	Material(std::string fn1, std::shared_ptr<Texture> tex)
	{
		m_shader = std::make_shared<Shader>("./res/SHADERS/" + fn1 + "Shader");
		m_textures.push_back(tex);
	}

	Material(std::shared_ptr<Shader> sdr, std::shared_ptr<Texture> tex)
	{
		m_shader = sdr;
		m_textures.push_back(tex);
	}

	void Update(Transform &transform, const Camera &camera, double time_passed)
	{
		if (m_shader == nullptr) { return; }
		m_shader->Bind();
		int i = 0;
		for (auto& t : m_textures)
		{
			if (t->IsLoaded())
				t->Bind(i++);
		}
		m_shader->Update(transform, camera, time_passed);
	}

	void Update(Transform &transform, const Camera &camera, double time_passed, const std::vector<Light*>& l)
	{
		if (m_shader == nullptr) { return; }
		m_shader->Bind();
		int i = 0;
		for (auto& t : m_textures)
		{
			if (t->IsLoaded())
				t->Bind(i++);
		}
		m_shader->Update(transform, camera, time_passed, l);
	}
	
	std::shared_ptr<Texture> GetTex() {return m_textures[0];}

	Material(const Material& other)
	{
		m_shader = other.m_shader;
		for ( auto& t : other.m_textures)
		m_textures.push_back(t);
	}
#if 0
	Material& operator=(const Material& other)
	{
		if (this == &other)
			return *this;

		m_shader = other.m_shader;
		m_texture = other.m_texture;

		return *this;
	}
#endif

	virtual ~Material()
	{
		//delete m_shader;
		//delete m_texture;
	}

	void Add(std::shared_ptr<Texture> t) { m_textures.push_back(t); }

private:
	std::shared_ptr<Shader>  m_shader;
	std::vector<std::shared_ptr<Texture>> m_textures;
};

