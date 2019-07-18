#pragma once

#include "include/GLEW/GL/glew.h"

#include "TextureAttachment.h"


class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void OnInit();

	void Bind();
	void Unbind();

	TextureAttachment GetColor() { return m_tex_color_buffer; }

private:
	bool m_bound;

	GLuint m_framebuffer;
	GLuint m_rbo;
	GLuint m_tex_color_buffer;

};

