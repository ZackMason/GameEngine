#version 460

in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec4 color;

out vec2 uv;

uniform mat4 ModelViewProjection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform vec3 LightPosition;
uniform float Time;


void main () 
{
	uv = vec2(position.x,position.y);
	gl_Position = (ViewMatrix * vec4(position,1.0)).xyz;
}