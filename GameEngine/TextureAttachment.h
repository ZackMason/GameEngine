#pragma once
#include "Texture.h"
class TextureAttachment :
	public Texture
{
public:
	TextureAttachment();
	~TextureAttachment();


	TextureAttachment& operator=(const TextureAttachment& other)
	{
		if (this != &other)
		{
			this->m_texture = other.m_texture;
			this->m_loaded = other.m_loaded;
		}
	}

};

