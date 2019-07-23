#pragma once

#include "include/GLEW/GL/glew.h"

#include "TextureAttachment.h"
#include "DepthAttachment.h"

#define FB_DEPTH_ATTACHMENT 1
#define FB_DEPTH_RENDERBUFFER 1<<1


class FrameBuffer
{
public:
	FrameBuffer(bool type);
	~FrameBuffer();

	void Bind();
	void Unbind();

	GLint GetTexUnit() { return GL_TEXTURE0 + 1; }
	GLuint GetBuffer() { return m_framebuffer; }
	GLuint GetColor() { return m_TA.GetTex(); }
	GLuint GetDColor() { return m_DTA.GetTex(); }
	GLuint GetDepth() { return m_DA.GetTex(); }

	void Bind_TA(int u) { m_TA.Bind(u); }
	void Bind_DTA(int u) { m_DTA.Bind(u); }

	operator unsigned int() { return m_framebuffer; }

private:
	bool m_bound;

	GLuint m_framebuffer;
	GLuint m_rbo;

	DepthAttachment m_DA;

	TextureAttachment m_TA;
	TextureAttachment m_DTA;
};

