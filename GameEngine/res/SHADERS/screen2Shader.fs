#version 460

layout (location = 0) uniform sampler2D diffuse;
layout (location = 1) uniform sampler2D depth;
uniform float Time;

in vec2 uv;

out vec4 color;

const vec2 res = vec2(1920,1080);

uniform float gamma;
uniform float expos;

void main ()
{

	color.rgb = texture(diffuse,uv).rgb;
	color.a = 1;
	//color.rg = uv;
}