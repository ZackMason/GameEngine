#version 460

uniform sampler2D diffuse;

in vec2 texCoord0;
in vec3 normal0;
out vec4 color;

void main()
{
	color = texture2D(diffuse, texCoord0);

}