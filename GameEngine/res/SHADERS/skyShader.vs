#version 460

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec3 normal0;
out vec2 texCoord0;

uniform mat4 ModelViewProjection;
uniform mat4 ViewMatrix;

void main()
{
	texCoord0 = texCoord;
	normal0 = (ViewMatrix * vec4(normal,0.0)).xyz;
	gl_Position = ModelViewProjection * vec4(position, 1.0);
}