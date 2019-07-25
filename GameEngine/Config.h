#pragma once
#include "imGUI/imgui.h"
#include "include/glm/glm.hpp"
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
	static float mesh_size;
	static glm::vec4 shader_color;
	static Config* s_Instance;
};

