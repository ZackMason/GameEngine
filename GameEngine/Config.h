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
	static float bias;
	static float scale;
	static float power;
	static float lp;
	static bool fly;
	static bool wire;
	static float mesh_size;
	static glm::vec4 shader_color;
	static glm::vec4 shader_color2;
	static glm::vec4 shader_color3;
	static glm::vec4 shader_color4;
	static Config* s_Instance;
};

