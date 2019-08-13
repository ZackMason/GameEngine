#version 460

uniform sampler2D diffuse;

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 Position_worldspace;

uniform float Time;

out vec4 color;
//layout (location = 1) out vec4 dcolor;

float rand(vec2 n){
	return fract(sin(dot(n,vec2(12.9898,4.1414)))*43758.5453);
}

float noise(vec2 p){
	vec2 ip=floor(p);
	vec2 u=fract(p);
	u=u*u*(3.-2.*u);
	
	float res=mix(
		mix(rand(ip),rand(ip+vec2(1.,0.)),u.x),
		mix(rand(ip+vec2(0.,1.)),rand(ip+vec2(1.,1.)),u.x),u.y);
	return res*res;
}

float grid(vec3 st,float res)
{
    vec3 grid=fract(st*res);
    return(step(res,grid.x)*step(res,grid.y)*step(res,grid.z));
}


void main()
{
	#if 0
	// Material properties
	//vec3 MaterialDiffuseColor = texture( diffuse, texCoord0 ).rgb;
	vec3 hor = vec3(1.5);

	vec3 MaterialDiffuseColor =mix(hor, vec3(0.1451, 0.549, 0.9255), min(Position_worldspace.y/600.,1.0)); 
	MaterialDiffuseColor = mix(MaterialDiffuseColor,MaterialDiffuseColor*.4,min(Position_worldspace.y/4500.,1.0));
	vec3 MaterialAmbientColor = vec3(0.21,0.21,0.21) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);
	
	float n1 = noise(texCoord0*800.);
	if(min((n1*.5)+.5,1.)>0.97 && Position_worldspace.y > 2000)
	{
		MaterialDiffuseColor = vec3(1);
	}
	#endif

	vec3 MaterialDiffuseColor = vec3(0.2157, 0.3922, 0.7725);
//	vec3 MaterialDiffuseColor = vec3(0.18,0.27,.47);
	MaterialDiffuseColor.r += -0.00022 * min(Position_worldspace.y - 1800,0.);
	MaterialDiffuseColor.g += -0.00025 * min(Position_worldspace.y - 1800,0.);
	MaterialDiffuseColor.b += -0.00019 * min(Position_worldspace.y - 1800,0.);

	color.a = 1;
	color.rgb = MaterialDiffuseColor*MaterialDiffuseColor*MaterialDiffuseColor*10.;//+ MaterialAmbientColor;
	//color.rgb = vec3(0.4588, 0.4588, 1.0);//+ MaterialAmbientColor;

	//dcolor = vec4(0);

}