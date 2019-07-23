#version 460

uniform sampler2D diffuse;

in vec2 texCoord0;
in vec3 normal0;
in vec3 view0;

out vec4 color;

void main()
{	
	vec3 cool = vec3(0.4,.2,0.98);
	vec3 warm = vec3(1.,1.,.1);

	vec3 ld = normalize(vec3(0.3,.7,0.8));

	float weight = .5f * (1. + dot(normalize(normal0), normalize(ld)));

	vec3 col = mix(cool,warm,weight);

	color = vec4(col,1.);
}