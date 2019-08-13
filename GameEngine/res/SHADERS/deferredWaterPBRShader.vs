#version 460

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec3 normal0;
out vec2 texCoord0;
out vec3 Position_worldspace;
out vec3 vd;
out mat3 TBN;

uniform mat4 ModelViewProjection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

uniform float Time;

vec3 Gerstner_Wave(	vec4 wave,vec3 p,inout vec3 tangent,inout vec3 binormal);

void main()
{
	Position_worldspace=(ModelMatrix*vec4(position,1)).xyz;
	vec3 vertexPosition_cameraspace=(ViewMatrix*ModelMatrix*vec4(position,1)).xyz;
	vd = normalize(vec3 (0) -vertexPosition_cameraspace);
	vec3 pos = Position_worldspace;
	//pos.y += 35.;
	vec3 tangent = vec3(1,0,0);
	vec3 binormal = vec3(0,0,-1);
	vec4 wave1 = vec4(0,1,.015,1260.);
	vec4 wave2 = vec4(-1,-1,.0145, 265.);
	vec4 wave3 = vec4(1,-1.6,.0125, 600.);
	vec4 wave4 = vec4(1,.6,.0125,430.);
	vec4 wave5 = vec4(-1.3,0,.0125,345.);
	vec4 wave6 = vec4(1.3,0,.025,245.);
	vec4 wave7 = vec4(.3,1.0,.025,345.);
	vec4 wave8 = vec4(-.3,-1.0,.025,245.);
	vec4 wave9 = vec4(-1.3,1.0,.025,245.);
	vec4 wave10 = vec4(-1.3,5.0,.0125,445.);
	vec4 wave11 = vec4(-1.3,0,.0125,645.);
	pos += Gerstner_Wave(wave1,pos,tangent,binormal);
	pos += Gerstner_Wave(wave2, pos, tangent, binormal);
	pos += Gerstner_Wave(wave3, pos,tangent,binormal);
	pos += Gerstner_Wave(wave4, pos, tangent, binormal);
	pos += Gerstner_Wave(wave5, pos, tangent, binormal);
	pos += Gerstner_Wave(wave6, pos, tangent, binormal);
	pos += Gerstner_Wave(wave7, pos, tangent, binormal);
	pos += Gerstner_Wave(wave8, pos, tangent, binormal);
	pos += Gerstner_Wave(wave9, pos, tangent, binormal);
	pos += Gerstner_Wave(wave10, pos, tangent, binormal);
	pos += Gerstner_Wave(wave11, pos, tangent, binormal);
	//pos.y /= 4.;
	normal0 = normalize(cross(normalize(tangent),normalize(binormal)));
	TBN = mat3(tangent,binormal,normal0);
	Position_worldspace = pos;
	texCoord0=texCoord;
	gl_Position=ModelViewProjection*(inverse(ModelMatrix)*vec4(pos,1.));
	
	//normal0=(transpose(inverse(ModelMatrix))*vec4(normal,0.)).xyz;
	//gl_Position=ModelViewProjection*vec4(position,1.);
}

vec3 Gerstner_Wave(
	vec4 wave,vec3 p,inout vec3 tangent,inout vec3 binormal
){
	float steepness=wave.z;
	float wavelength=wave.w;
	float k=2*3.141592/wavelength;
	float c=sqrt(9.8/k);
	vec2 d=normalize(wave.xy);
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