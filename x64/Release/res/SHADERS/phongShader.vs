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


void main()
{
	//fragColor = color;
	Position_worldspace = (ModelMatrix * vec4(position,1)).xyz;

	vec3 vertexPosition_cameraspace = ( ViewMatrix * ModelMatrix * vec4(position,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	vec3 LightPosition_cameraspace = ( ViewMatrix * vec4(LightPosition,1)).xyz;
	LightDirection_cameraspace = normalize(LightPosition_cameraspace + EyeDirection_cameraspace);

	position0 = (ModelMatrix * vec4(position,0.0)).xyz;
	texCoord0 = texCoord;
	normal0 = (ViewMatrix * ModelMatrix * vec4(normal,0.0)).xyz;
	gl_Position = ModelViewProjection * vec4(position, 1.0);
}