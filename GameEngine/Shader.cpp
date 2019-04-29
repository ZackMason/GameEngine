#include "Shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(const std::string& fileName)
{
	m_program = glCreateProgram();
	m_fileName = fileName;
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(m_program, m_shaders[i]);
	}

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");

	glLinkProgram   (m_program);
	CheckShaderError(m_program, GL_LINK_STATUS,     true, "Error: Program linking failed: ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

	m_uniforms[MVP_U]   = glGetUniformLocation(m_program, "ModelViewProjection");
	m_uniforms[VIEW_U]  = glGetUniformLocation(m_program, "ViewMatrix");
	m_uniforms[MODEL_U] = glGetUniformLocation(m_program, "ModelMatrix");
	m_uniforms[LIGHT_U] = glGetUniformLocation(m_program, "LightPosition");
	m_uniforms[TIME_U]  = glGetUniformLocation(m_program, "Time");
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	glDeleteProgram(m_program);
}

std::string Shader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv (shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog (shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type);

	if (shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

	const GLchar* p[1];
	p[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource (shader, 1, p, lengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}

void Shader::Bind()
{
	glUseProgram(m_program);
}

void Shader::Update(const Transform& transform, const Camera& camera, double time_passed)
{
	glm::mat4 ModelMatrix         = transform.GetModel();
	glm::mat4 ViewMatrix          = camera.GetViewProjection();
	glm::mat4 ModelViewProjection = camera.GetViewProjection() * transform.GetModel();
	glm::vec3 LightPosition       = glm::vec3(sinf(time_passed) * 6.0f, sinf(time_passed) + 4.0f, cosf(time_passed) * 6.0f) + glm::vec3(-40,0,-40);

	//transpose if matrices are weird
	glUniformMatrix4fv              (m_uniforms[MVP_U]  , 1, GL_FALSE, &ModelViewProjection[0][0]);
	glUniformMatrix4fv              (m_uniforms[VIEW_U] , 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv              (m_uniforms[MODEL_U], 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniform3f		                (m_uniforms[LIGHT_U], LightPosition.x, LightPosition.y, LightPosition.z);
	glUniform1f		                (m_uniforms[TIME_U] , time_passed);
}