#pragma once
class Config
{
public:
	Config();
	~Config();

	static inline Config& Get() { return *s_Instance; }

	static float playerSpeed;
	static float shine;
	static float gamma;
	static float expos;
	static Config* s_Instance;
};

