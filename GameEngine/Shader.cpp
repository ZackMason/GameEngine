#include "Shader.h"
#include "Application.h"
#include "Config.h"
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

	glBindFragDataLocation(m_program, 0, "color");
	glBindFragDataLocation(m_program, 1, "dcolor");

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");
	//glBindAttribLocation(m_program, 3, "tangent");
	//glBindAttribLocation(m_program, 4, "bitangent");

	glLinkProgram   (m_program);
	CheckShaderError(m_program, GL_LINK_STATUS,     true, "Error: Program linking failed: ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

	GetUniformLocation("ModelViewProjection");
	GetUniformLocation("ViewMatrix");
	GetUniformLocation("ModelMatrix");
	GetUniformLocation("LightPosition");
	GetUniformLocation("Time");
	GetUniformLocation("diffuse");
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
	lengths[0] = static_cast<GLint>(text.length());

	glShaderSource (shader, 1, p, lengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}

void Shader::Bind()
{
	glUseProgram(m_program);
}

#if 1
void Shader::SetMVP( Transform& transform, const Camera& camera)
{
	glm::mat4 ModelMatrix         = transform.GetModel();
	glm::mat4 ViewMatrix          = camera.GetViewProjection();
	glm::mat4 ModelViewProjection = ViewMatrix * ModelMatrix;
	//transpose if matrices are weird
	//ModelViewProjection = glm::transpose(ModelViewProjection);
	//ModelMatrix = glm::transpose(ModelMatrix);
	//ViewMatrix = glm::transpose(ViewMatrix);
	//ModelViewProjection = glm::transpose(ModelViewProjection);
	glUniformMatrix4fv              (GetUniformLocation("ModelViewProjection"), 1, GL_FALSE, &ModelViewProjection[0][0]);
	glUniformMatrix4fv              (GetUniformLocation("ViewMatrix"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv              (GetUniformLocation("ModelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
}


void Shader::SetLights(const std::vector<Light*>& lights)
{

	setInt("num_lights", static_cast<int>(lights.size()));
	setFloat("material.shininess", Config::Get().shine);

	GLuint i = 0;
	for (auto&l : lights)
	{
		int j = 0;
		std::string number = std::to_string(i);
		glUniform4f(GetUniformLocation( (Application::Get().light_names[i][j++]).c_str()), lights[i]->m_position.x, lights[i]->m_position.y, lights[i]->m_position.z, lights[i]->m_position.w);
		glUniform4f(GetUniformLocation( (Application::Get().light_names[i][j++]).c_str()), lights[i]->m_color.x, lights[i]->m_color.y, lights[i]->m_color.z, lights[i]->m_position.w);
		glUniform1f(GetUniformLocation( (Application::Get().light_names[i][j++]).c_str()), lights[i]->m_ads.r);
		glUniform1f(GetUniformLocation( (Application::Get().light_names[i][j++]).c_str()), lights[i]->m_ads.g);
		glUniform1f(GetUniformLocation( (Application::Get().light_names[i][j++]).c_str()), lights[i]->m_ads.b);
		glUniform1f(GetUniformLocation( (Application::Get().light_names[i][j++]).c_str()), lights[i]->m_clq.x);
		glUniform1f(GetUniformLocation( (Application::Get().light_names[i][j++]).c_str()), lights[i]->m_clq.y);
		glUniform1f(GetUniformLocation( (Application::Get().light_names[i][j++]).c_str()), lights[i]->m_clq.z);
		i++;
	}
}
#endif

GLint Shader::GetUniformLocation(const std::string& name) const
{
	if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
		return m_uniform_location_cache[name];
	GLint location = glGetUniformLocation(m_program, name.c_str());
	m_uniform_location_cache[name] = location;
	return location;
}