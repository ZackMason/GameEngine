#version 460

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 normal0;
out vec2 texCoord0;
out vec3 EyeDirection_cameraspace;
out vec3 Position_worldspace;
out vec3 vertexPosition_cameraspace;
//out vec3 position0;
//out vec3 color0;

uniform mat4 ModelViewProjection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
	//position0 = position;
	//position0 = vec3(ModelViewProjection * vec4(position, 1.0));
	//position0 = vec3(ModelMatrix * vec4(position, 1.0));
	Position_worldspace = (ModelMatrix * vec4(position,1)).xyz;

	normal0 = normal;
	texCoord0 = texCoord;
	vertexPosition_cameraspace = ( ViewMatrix * ModelMatrix * vec4(position,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
	//color0 = color;
	gl_Position = ModelViewProjection * vec4(position, 1.0);
}