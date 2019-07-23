#include "LightShader.h"


LightShader::~LightShader()
{
}

#if 1 
void LightShader::Update(const Transform & transform, const Camera & camera, double time_passed, const std::vector<Light*>& lights)
{
	glm::mat4 ModelMatrix = transform.GetModel();
	glm::mat4 ViewMatrix = camera.GetViewProjection();
	glm::mat4 ModelViewProjection = camera.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(m_uniforms[MVP_U], 1, GL_FALSE, &ModelViewProjection[0][0]);
	glUniformMatrix4fv(m_uniforms[VIEW_U], 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(m_uniforms[MODEL_U], 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniform1f(m_uniforms[TIME_U], time_passed);

	setInt("material.diffuse", 0);
	setInt("material.specular", 1);
	setFloat("material.shininess", 32.0f);

	GLuint i = 0;
	for (auto&l:lights)
	{
		std::string number = std::to_string(i);

		glUniform4f(glGetUniformLocation(m_program, ("pointLight[" + number + "].position").c_str()), lights[i]->m_position.x, lights[i]->m_position.y, lights[i]->m_position.z, lights[i]->m_position.w);
		glUniform4f(glGetUniformLocation(m_program, ("pointLight[" + number + "].color").c_str()), lights[i]->m_position.x, lights[i]->m_position.y, lights[i]->m_position.z, lights[i]->m_position.w);
		glUniform3f(glGetUniformLocation(m_program, ("pointLight[" + number + "].ambient").c_str()) , lights[i]->m_ads.r, lights[i]->m_ads.r, lights[i]->m_ads.r);
		glUniform3f(glGetUniformLocation(m_program, ("pointLight[" + number + "].diffuse").c_str()) , lights[i]->m_ads.g, lights[i]->m_ads.g, lights[i]->m_ads.g);
		glUniform3f(glGetUniformLocation(m_program, ("pointLight[" + number + "].specular").c_str()), lights[i]->m_ads.b, lights[i]->m_ads.b, lights[i]->m_ads.b);
		glUniform1f(glGetUniformLocation(m_program, ("pointLight[" + number + "].constant").c_str()), lights[i]->m_clq.x);
		glUniform1f(glGetUniformLocation(m_program, ("pointLight[" + number + "].linear").c_str()), lights[i]->m_clq.y);
		glUniform1f(glGetUniformLocation(m_program, ("pointLight[" + number + "].quadratic").c_str()), lights[i]->m_clq.z);
		i++;
	}
}
#endif
