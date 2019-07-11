#pragma once

#include <string>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Transform.h"

class Material
{
public:
	Material();

	Material(std::string filename) 
	{
		m_shader  = std::make_shared< Shader >("./res/SHADERS/"  + filename + "Shader");
		m_texture = std::make_shared< Texture>("./res/TEXTURES/" + filename + ".jpg"  );
		if (!m_texture->IsLoaded())
			m_texture = std::make_shared<Texture>("./res/TEXTURES/" + filename + ".png");
	}

	Material(std::string fn1, std::string fn2)
	{
		m_shader  = std::make_shared<Shader>("./res/SHADERS/"  + fn1 + "Shader");
		m_texture = std::make_shared<Texture>("./res/TEXTURES/" + fn2 + ".jpg"  );
		if (!m_texture->IsLoaded())
			m_texture =	std::make_shared<Texture>("./res/TEXTURES/" + fn2 + ".png");
	}

	Material(std::string fn1, std::string fn2, std::string fn3)
	{
		m_shader  =  std::make_shared<Shader> ("./res/SHADERS/"  + fn1 + "Shader");
		m_texture = std::make_shared<Texture>("./res/TEXTURES/" + fn2 + fn3     );
	}

	Material(std::string fn1, std::shared_ptr<Texture> tex)
	{
		m_shader = std::make_shared<Shader>("./res/SHADERS/" + fn1 + "Shader");
		m_texture = tex;
	}

	Material(std::shared_ptr<Shader> sdr, std::shared_ptr<Texture> tex)
	{
		m_shader = sdr;
		m_texture = tex;
	}

	void Update( Transform &transform, const Camera &camera, double time_passed)
	{
		if (m_shader == nullptr) { return; }
		m_shader->Bind();
		if (m_texture->IsLoaded())
			m_texture->Bind(0);
		m_shader->Update(transform, camera, time_passed);
	}

#if 0
	Material(const Material& other)
	{
		m_shader = other.m_shader;
		m_texture = other.m_texture;
	}
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

private:
	std::shared_ptr<Shader>  m_shader;
	std::shared_ptr<Texture> m_texture;
};

