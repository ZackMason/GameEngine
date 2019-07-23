#pragma once
#include "Shader.h"

#include <vector>
#include "Light.h"

class LightShader :
	public Shader
{
public:
	LightShader(const std::string& fileName) : Shader(fileName) {};
	~LightShader();

	void Update(const Transform& transform, const Camera& camera, double time_passed, const std::vector<Light*>& lights);
};

