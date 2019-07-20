#pragma once

#include "include/GLEW/GL/glew.h"
#include <iostream>
#include <string>

class Texture
{
public:
	Texture();
	Texture(const std::string& fileName);

	GLuint GetTex() { return m_texture; }

	void Bind(unsigned int unit);

	bool IsLoaded() { return m_loaded; }

	virtual ~Texture();

	operator GLuint() { return m_texture; std::cout << "CAST TEXTURE\n"; }

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
protected:

	GLuint m_texture;
	bool m_loaded;
};

