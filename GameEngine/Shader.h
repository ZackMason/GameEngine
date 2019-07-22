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

	// utility uniform functions
   // ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_program, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_program, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_program, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(m_program, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}


	typedef void(*fnPtr)();
	fnPtr updatefn = nullptr;

	void setupdate(fnPtr fn) { updatefn = fn; }
	void callupdate() { updatefn(); }


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

