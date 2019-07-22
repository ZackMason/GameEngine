#include "Config.h"


Config* Config::s_Instance = nullptr;

Config::Config()
{
	s_Instance = this;
}

float Config::playerSpeed = 30.0;

Config::~Config()
{
}
