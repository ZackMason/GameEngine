#pragma once
class Config
{
public:
	Config();
	~Config();

	static inline Config& Get() { return *s_Instance; }

	static float playerSpeed;
	static Config* s_Instance;
};

