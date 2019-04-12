#version 460

uniform sampler2D diffuse;

in vec2 texCoord0;
in vec3 normal0;
in vec3 view0;
in float r0;
in vec3 position0;
in vec4 col0;

out vec4 color;

void main()
{	
	vec3 col1 = vec3(.0,0.,0.);
	vec3 col2 = vec3(.8,.8,1.);
	

	color = col0;//mix(col1,col2, r0),1.);
}