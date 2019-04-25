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
		m_shader  = new Shader ("./res/SHADERS/"  + filename + "Shader");
		m_texture = new Texture("./res/TEXTURES/" + filename + ".jpg"  );
		if (!m_texture->IsLoaded())
			m_texture = new Texture("./res/TEXTURES/" + filename + ".png");
	}

	Material(std::string fn1, std::string fn2)
	{
		m_shader  = new Shader ("./res/SHADERS/"  + fn1 + "Shader");
		m_texture = new Texture("./res/TEXTURES/" + fn2 + ".jpg"  );
		if (!m_texture->IsLoaded())
			m_texture = new Texture("./res/TEXTURES/" + fn2 + ".png");
	}

	Material(std::string fn1, std::string fn2, std::string fn3)
	{
		m_shader  = new Shader ("./res/SHADERS/"  + fn1 + "Shader");
		m_texture = new Texture("./res/TEXTURES/" + fn2 + fn3     );
	}

	void Update(const Transform &transform, const Camera &camera, double time_passed)
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
	Shader  *m_shader;
	Texture *m_texture;
};
