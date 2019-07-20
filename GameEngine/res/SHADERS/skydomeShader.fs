#version 460

uniform sampler2D diffuse;

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 Position_worldspace;

uniform vec3 LightPosition;
uniform float Time;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 dcolor;

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
	vec3 LightColor = vec3(1,0.1,0.1);
	vec3 col2 = vec3 (0.,.0,1.);
	LightColor = mix(LightColor,col2, abs(sin(Time)));
	LightColor = vec3(1);
	float LightPower = 50.0f;
	
	// Material properties
	//vec3 MaterialDiffuseColor = texture( diffuse, texCoord0 ).rgb;
	vec3 hor = vec3(1.5);

	vec3 MaterialDiffuseColor =mix(hor, vec3(0.1451, 0.549, 0.9255), min(Position_worldspace.y/600.,1.0)); 
	MaterialDiffuseColor = mix(MaterialDiffuseColor,MaterialDiffuseColor*.4,min(Position_worldspace.y/4500.,1.0));
	vec3 MaterialAmbientColor = vec3(0.21,0.21,0.21) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);
	
	float n1 = noise(texCoord0*800.);
	float n2 = noise(Position_worldspace.xy/500.);
	float n3 = noise(Position_worldspace.zy/500.);
	float n4 = noise(Position_worldspace.zx/300.);
	float r1 = n1+n2+n3+n4;
	if(min((n1*.5)+.5,1.)>0.97 && Position_worldspace.y > 2000)
	{
		MaterialDiffuseColor = vec3(1);
	}
	//MaterialDiffuseColor *= vec3(1.)-vec3(grid(abs(Position_worldspace/6.),0.05));
	// Distance to the light
	#if 0
	float distance = length( LightPosition - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( normal0 );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	color.rgb = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
	#endif
	color.a = 1;
	color.rgb = MaterialDiffuseColor + MaterialAmbientColor;
	color.rgb=1.-exp(-1.*color.rgb);
	dcolor = vec4(1);

}