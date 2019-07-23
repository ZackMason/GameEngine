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
	#if 0
	//color.rgb = color.rgb/(1.+color.rgb);
	vec2 map = vec2(0.65,-0.3);
	vec2 p = (uv*res.xy)/res.y - (vec2(.5)*res.xy)/res.y;
	vec2 pp = map - p;
	if(length(pp) < 0.1)
	{
		if (dot(normalize(pp), normalize(vec2(cos(Time),sin(Time)))) > .999)
			color.rgb = vec3(0,1,0);
		else
		color.rgb = vec3(0);
	}
	//color.rgb = vec3(dot(pp,vec2(cos(Time),sin(Time))));

	//float bright = dot(color.rgb,vec3(0.2627,0.678,.0593));
	//color.rgb *= bright;
	float d = texture(depth,uv).r;
	#endif

	//vec3 mapped = color.rgb / (color.rgb + vec3(1.0));
    // gamma correction 
	vec3 mapped = vec3(1.0) - exp(-color.rgb * expos);
    mapped = pow(mapped, vec3(1.0 / gamma));
	color.rgb = mapped;
//	color.rgb=texture(diffuse,uv).rgb;

	color.a = 1;
	//color.rg = uv;
}