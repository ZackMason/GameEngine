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

uniform mat4 ModelViewProjection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform vec3 LightPosition;
uniform float Time;

float calcxWave(float x)
{
	float y=(sin(x*1.+Time*1.)+sin(x*2.3+Time*1.5)+sin(x*3.3+Time*.4))/3.;
	return y;
}

float calcyWave(float x)
{
	float y=(sin(x*1.+Time*1.)+sin(x*2.8+Time*1.5)+sin(x*3.9+Time*.4))/3.;
	return y;
}

void main()
{
	Position_worldspace = (ModelMatrix * vec4(position,1)).xyz;

	vec3 vertexPosition_cameraspace = ( ViewMatrix * ModelMatrix * vec4(position,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	vec3 LightPosition_cameraspace = ( ViewMatrix * vec4(LightPosition,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	position0 = (ModelMatrix * vec4(position,1.0)).xyz;
	texCoord0 = texCoord;
	normal0 = (ViewMatrix * ModelMatrix * vec4(normal,0.0)).xyz;
	vec3 pos = position;
	pos.y += calcxWave(Position_worldspace.x*.01)*3.;
	pos.y += calcyWave(Position_worldspace.z*.01)*3.;
	gl_Position = ModelViewProjection * vec4(pos, 1.0);
}