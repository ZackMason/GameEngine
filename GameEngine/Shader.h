#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "include/GLEW/GL/glew.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"


class Shader
{
public:
	Shader(const std::string& fileName);

	void Bind  ();
	void SetMVP(Transform& transform, const Camera& camera);
	void SetLights(const std::vector<Light*>& lights = { 0 });


	virtual ~Shader();

#if 1
	// utility uniform functions
   // ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(GetUniformLocation( name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(GetUniformLocation( name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(GetUniformLocation( name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(GetUniformLocation( name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(GetUniformLocation( name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(GetUniformLocation( name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(GetUniformLocation( name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(GetUniformLocation( name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(GetUniformLocation( name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(GetUniformLocation( name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(GetUniformLocation( name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(GetUniformLocation( name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
#endif

	// public cache of uniform locations
	mutable std::unordered_map<std::string, GLint> m_uniform_location_cache;

	// Fucntion Definitions
protected:
	Shader(const Shader& other) = default;
	Shader& operator=(const Shader& other) = default;

	std::string LoadShader       (const std::string& fileName);
	GLuint      CreateShader     (const std::string& text    , unsigned int type);
	void        CheckShaderError (           GLuint  shader  ,       GLuint flag, bool isProgram, const std::string& errorMessage);
	GLint		GetUniformLocation(const std::string& name) const;

	// Member Varible Declarations
protected:
	static const unsigned int NUM_SHADERS = 2;
	std::string m_fileName;

	GLuint m_program;
	GLuint m_shaders [ NUM_SHADERS ];

};

