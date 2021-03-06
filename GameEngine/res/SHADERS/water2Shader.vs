#version 460

in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec4 color;

out vec3 normal0;
out vec2 texCoord0;
out vec3 position0;
out vec3 LightDirection_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 Position_worldspace;
out mat3 TBN;

uniform mat4 ModelViewProjection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform vec3 LightPosition;
uniform float Time;
uniform float power;
uniform float DT;

vec3 gerstnerWave(in vec3 pos, in vec2 d )
{
	float a=5.;
	float l=2.;
	float w=2.*3.1416/l;
	float q=.1;
	
	float dd=dot(pos.xz,d);
	
	float s=sin(w*dd+Time);
	float c=cos(w*dd+Time);
	
	vec3 p=vec3(pos.x+q*a*c*d.x,a*s,pos.z+q*a*c*d.y);
	return p;
}

vec3 Gerstner_Wave(
	vec4 wave,vec3 p,inout vec3 tangent,inout vec3 binormal
){
	float steepness=wave.z;
	float wavelength=wave.w;
	float k=2*3.141592/wavelength;
	float c=sqrt(9.8/k);
	vec2 d= normalize(wave.xy);
	float f=k*(dot(d,p.xz)-c*Time*3.);
	float a=steepness/k;

	//p.x += d.x * (a * cos(f));
	//p.y = a * sin(f);
	//p.z += d.y * (a * cos(f));
	
	tangent+=vec3(
		-d.x*d.x*(steepness*sin(f)),
		d.x*(steepness*cos(f)),
		-d.x*d.y*(steepness*sin(f))
	);
	binormal+=vec3(
		-d.x*d.y*(steepness*sin(f)),
		d.y*(steepness*cos(f)),
		-d.y*d.y*(steepness*sin(f))
	);
	return vec3(
		d.x*(a*cos(f)),
		a*sin(f),
		d.y*(a*cos(f))
	);
}

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
void main()
{
	Position_worldspace = (ModelMatrix * vec4(position,1)).xyz;

	vec3 vertexPosition_cameraspace = ( ViewMatrix * ModelMatrix * vec4(position,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	vec3 l = vec3(0,100,0);
	vec3 LightPosition_cameraspace = ( ViewMatrix * vec4(l,1)).xyz;
	//vec3 LightPosition_cameraspace = ( ViewMatrix * vec4(LightPosition,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	position0 = (ModelMatrix * vec4(position,1.0)).xyz;
	texCoord0 = texCoord;
	vec3 pos = Position_worldspace;
	//pos.y += 35.;
	vec3 tangent = vec3(1,0,0);
	vec3 binormal = vec3(0,0,-1);
	vec4 wave1 = vec4(0,1,.15,1260.);
	vec4 wave2 = vec4(-1,-1,.145, 1265.);
	vec4 wave3 = vec4(1,-1.6,.125, 1600.);
	vec4 wave4 = vec4(1,.6,.125,430.);
	vec4 wave5 = vec4(-1.3,0,.125,845.);
	pos += Gerstner_Wave(wave1,pos,tangent,binormal);
	pos += Gerstner_Wave(wave2, pos, tangent, binormal);
	pos += Gerstner_Wave(wave3, pos,tangent,binormal);
	pos += Gerstner_Wave(wave4, pos, tangent, binormal);
	pos += Gerstner_Wave(wave5, pos, tangent, binormal);

	#if 0
	for (float i = 0.; i < 5.; i += 1.)
	{
		vec4 wave = vec4(1.,0.,1./20.,1000.*sin(i));
		pos += Gerstner_Wave(wave, pos, tangent, binormal);
	}
	#endif

	normal0 = normalize(cross(tangent,binormal));
	TBN = mat3(tangent,binormal,normal0);
	Position_worldspace = pos;
	//normal0 = (ViewMatrix * ModelMatrix * vec4(normal0,0.0)).xyz;
	//pos = gerstnerWave(pos,wavedir.xy)+gerstnerWave(pos,wavedir.yx)+gerstnerWave(pos,wavedir.yy);
	gl_Position = ModelViewProjection*(inverse(ModelMatrix) * vec4(pos, 1.0));
	//gl_Position = ModelViewProjection * vec4(pos, 1.0);
}