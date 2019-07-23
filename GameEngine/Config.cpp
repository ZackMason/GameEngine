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

Config::~Config()
{
}
