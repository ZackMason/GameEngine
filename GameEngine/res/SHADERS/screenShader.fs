#version 460

layout (location = 0) uniform sampler2D diffuse;
layout (location = 1) uniform sampler2D depth;
uniform float Time;

in vec2 uv;

out vec4 color;

const vec2 res = vec2(1920,1080);

uniform float gamma;
uniform float expos;

vec3 Uncharted2Tonemap(vec3 x)
{
    float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;

    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec3 gammaCorrection(vec3 color){
	// gamma correction 
	color = pow(color, vec3(1.0/gamma)); 
	return color;
}

vec3 Reinhard(vec3 color)
{
	return color.rgb / (color.rgb + vec3(1.0));
}

vec3 Exposure(vec3 color) 
{
	return vec3(1.0) - exp(-color.rgb * expos);
}

void main ()
{

	color.rgb = texture(diffuse,uv).rgb;
	#if 0


	float d = texture(depth,uv).r;

	//vec3 mapped = color.rgb / (color.rgb + vec3(1.0));
    // gamma correction 
	color.rgb = vec3(1) * bright;

	
	#endif
	float bright = dot(color.rgb,vec3(0.2627,0.678,.0593));


	#if 0
	color.rgb = Reinhard(color.rgb);
	//color.rgb = Exposure(color.rgb);
	color.rgb = gammaCorrection(color.rgb);
	#endif

	color.a = 1;
	//color.rg = uv;
}