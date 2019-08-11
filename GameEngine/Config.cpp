#include "Config.h"


Config* Config::s_Instance = nullptr;

Config::Config()
{
	s_Instance = this;
}

float Config::playerSpeed = 30.0;
float Config::shine = 30.0;
float Config::gamma = 2.20f;
float Config::expos = 1.720f;
float Config::bias = 0.001f;
float Config::scale = 0.147920f;
float Config::power = 1.3666f;
float Config::lp = 1.0f;
float Config::mesh_size = 100.f;
bool Config::fly = false;
bool Config::wire = false;
glm::vec4 Config::shader_color = glm::vec4 (198.f/255.f, 255.f / 255.f, 236.f / 255.f,1.f);
glm::vec4 Config::shader_color2 = glm::vec4(13.f/255.f, 13.f / 255.f, 179.f / 255.f, 1.f);
glm::vec4 Config::shader_color3 = glm::vec4(6.f/255.f, 6.f / 255.f, 94.f / 255.f, 1.f);
glm::vec4 Config::shader_color4 = glm::vec4(4.f/255.f, 4.f / 255.f, 253.f / 255.f, 1.f);

Config::~Config()
{
}
