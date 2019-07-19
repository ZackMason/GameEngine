#pragma once

#include "include/GLEW/GL/glew.h"

#include "TextureAttachment.h"


class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind();
	void Unbind();

	GLint GetTexUnit() { return GL_TEXTURE0 + 1; }
	GLuint GetBuffer() { return m_framebuffer; }
	GLuint GetColor() { return m_tex_color_buffer; }

	operator unsigned int() { return m_framebuffer; }

private:
	bool m_bound;

	GLuint m_framebuffer;
	GLuint m_rbo;
	GLuint m_tex_color_buffer;

};

