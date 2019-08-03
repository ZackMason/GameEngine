#include "Config.h"


Config* Config::s_Instance = nullptr;

Config::Config()
{
	s_Instance = this;
}

float Config::playerSpeed = 30.0;
float Config::shine = 30.0;
float Config::gamma = 2.20f;
float Config::expos = 1.20f;
float Config::bias = 0.001f;
float Config::scale = 0.47920f;
float Config::power = 1.3666f;
float Config::mesh_size = 100.f;
bool Config::fly = false;
glm::vec4 Config::shader_color = glm::vec4(1, 0, 0, 1);

Config::~Config()
{
}
