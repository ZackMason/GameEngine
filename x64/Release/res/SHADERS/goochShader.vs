#version 460

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec3 normal0;
out vec2 texCoord0;
out vec3 view0;
out float r0;

uniform mat4 ModelViewProjection;

void main()
{
	vec3 eye = vec3(0, 0, 5);
	view0 = (ModelViewProjection * vec4(eye-position, 0.0)).xyz;
	normal0 = (ModelViewProjection * vec4(normal,0.0)).xyz;

	texCoord0 = texCoord;
	gl_Position = ModelViewProjection * vec4(position, 1.0);
	//gl_PointSize = gl_Position.z;
}
