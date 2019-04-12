#pragma once

#include "include/GLEW/GL/glew.h"
#include <string>

class Texture
{
public:
	Texture();
	Texture(const std::string& fileName);

	void Bind(unsigned int unit);

	bool IsLoaded() { return m_loaded; }

	virtual ~Texture();

private:
	Texture(const Texture& other)
	{
		m_texture = other.m_texture;
		m_loaded = other.m_loaded;
	}

	Texture& operator=(const Texture& other)
	{
		if (this != &other)
		{
			this->m_texture = other.m_texture;
			this->m_loaded = other.m_loaded;
		}
	}

	GLuint m_texture;

	bool m_loaded;
};

