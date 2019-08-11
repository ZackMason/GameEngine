#version 460

layout (location = 0) uniform sampler2D diffuse;
layout (location = 1) uniform sampler2D depth;
uniform float Time;

in vec2 uv;

out vec4 color;

const vec2 res = vec2(1920,1080);

uniform float gamma;
uniform float expos;


vec3 Uncharted2Tonemap(vec3 x);
vec3 gammaCorrection(vec3 color);
vec3 Reinhard(vec3 color);
vec3 Exposure(vec3 color);
vec3 run_kernel(float[9]kernel);

float den=4.;
float blur[9]=float[](
	1./den,2./den,1./den,
	2./den,4./den,2./den,
	1./den,2./den,1./den
);

void main ()
{

	color.rgb = texture(depth,uv).rgb;
	#if 0


	float d = texture(depth,uv).r;

	//vec3 mapped = color.rgb / (color.rgb + vec3(1.0));
    // gamma correction 
	color.rgb = vec3(1) * bright;

	
	#endif
	float bright = dot(color.rgb,vec3(0.2627,0.678,.0593));


	//color.rgb *= run_kernel(blur);
	#if 1
	if(gamma < 1.0)
		color.rgb = Reinhard(color.rgb);
	else
		color.rgb = Uncharted2Tonemap(color.rgb);
	color.rgb = Exposure(color.rgb);
 	color.rgb = gammaCorrection(color.rgb);
	#endif

	//color.rgb = vec3(bright);
	color.a = 1;
	//color.rg = uv;
}

vec3 Uncharted2Tonemap(vec3 x)
{
	float A=.15;
	float B=.50;
	float C=.10;
	float D=.20;
	float E=.02;
	float F=.30;
	
	return((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec3 gammaCorrection(vec3 color){
	// gamma correction
	color=pow(color,vec3(1./2.2));
	return color;
}

vec3 Reinhard(vec3 color)
{
	return color.rgb/(color.rgb+vec3(1.));
}

vec3 Exposure(vec3 color)
{
	return vec3(1.)-exp(-color.rgb*expos);
}



float offset=1./400.;

vec2 offsets[9]=vec2[](
	vec2(-offset,offset),// top-left
	vec2(0.f,offset),// top-center
	vec2(offset,offset),// top-right
	vec2(-offset,0.f),// center-left
	vec2(0.f,0.f),// center-center
	vec2(offset,0.f),// center-right
	vec2(-offset,-offset),// bottom-left
	vec2(0.f,-offset),// bottom-center
	vec2(offset,-offset)// bottom-right
);

vec3 run_kernel(float[9]kernel)
{
	vec3 sampleTex[9];
	vec3 res=vec3(0.);
	for(int i=0;i<9;i++)
		sampleTex[i]=vec3(texture(diffuse,uv.st+offsets[i]));
	for(int i=0;i<9;i++)
	{
		res+=sampleTex[i]*kernel[i];
	}
	return res;
}