#version 460

uniform sampler2D diffuse;

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;
uniform vec3 LightPosition;
uniform float Time;

out vec4 color;

void main()
{
	vec3 warm = vec3(1.,.6,.1);
	vec3 cool = vec3(.4,.1,1.);
	float weight = 0.5 * (1.0 + dot(normalize(normal0), normalize(LightDirection_cameraspace)));
  	vec3 lightColor = mix(cool, warm, weight);
  	color = vec4(lightColor, 1);
}