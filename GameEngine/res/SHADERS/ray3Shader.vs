#version 460

in vec3 position;
in vec2 texCoord;

out vec2 iuv;

void main () 
{
	iuv = texCoord;
	gl_Position = vec4(position.x,position.y, 0.0 ,1.0);
}