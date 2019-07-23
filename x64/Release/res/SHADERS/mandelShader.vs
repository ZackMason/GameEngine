#version 460

in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec4 color;

out vec3 normal0;
out vec2 texCoord0;
out vec3 position0;
out vec3 LightDirection_cameraspace;
out vec3 EyeDirection_cameraspace;

uniform mat4 ModelViewProjection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform vec3 LightPosition;
uniform float Time;

void main()
{
	texCoord0 = texCoord;
	gl_Position = ModelViewProjection * vec4(position, 1.0);
}