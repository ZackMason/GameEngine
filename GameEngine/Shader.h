#pragma once
#include <string>
#include "include/GLEW/GL/glew.h"
#include "Transform.h"
#include "Camera.h"

class Shader
{
public:
	Shader();
	Shader(const std::string& fileName);

	void Bind  ();
	void Update(Transform& transform, const Camera& camera, double time_passed);

	virtual ~Shader();

	void SetInt(const std::string &name, int val) const
	{
		glUniform1i(glGetUniformLocation(m_program, name.c_str()), val);
	}

	int setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
		return glGetUniformLocation(m_program, name.c_str());
	}

private:
	static const unsigned int NUM_SHADERS = 2;
	Shader(const Shader& other)
	{
		m_fileName = other.m_fileName;
		m_program  = other.m_program;
	}
#if 0
	Shader(const Shader& other)
	{
		if (this != &other)
		{
			m_program = other.m_program;
			for (unsigned int i = 0; i < NUM_SHADERS; i++)
			{
				m_shaders[i] = other.m_shaders[i];
				//glDetachShader(other.m_program, other.m_shaders[i]);
				//glDeleteShader(other.m_shaders[i]);
			}
			//glDeleteProgram(other.m_program);
		}
	}
#endif
	Shader& operator=(const Shader& other) 
	{
		if (this != &other)
		{
			m_program = other.m_program;
			for (unsigned int i = 0; i < NUM_SHADERS; i++)
			{
				m_shaders[i] = other.m_shaders[i];
				glDetachShader(m_program, m_shaders[i]);
				glDeleteShader(m_shaders[i]);
			}
			glDeleteProgram(m_program);

		}
		return *this;
	}

	enum
	{
		MVP_U   ,
		VIEW_U  ,
		MODEL_U ,
		LIGHT_U ,
		TIME_U  ,
		DIFFUSE_U,

		NUM_UNIFORMS
	};

	std::string LoadShader       (const std::string& fileName);
	GLuint      CreateShader     (const std::string& text    , unsigned int type);
	void        CheckShaderError (           GLuint  shader  ,       GLuint flag, bool isProgram, const std::string& errorMessage);

	std::string m_fileName;

	GLuint m_program;
	GLuint m_shaders [ NUM_SHADERS ];
	GLuint m_uniforms[ NUM_UNIFORMS];
};

