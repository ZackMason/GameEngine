#version 460

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec3 normal0;
out vec2 texCoord0;
out vec3 position0;
out vec3 Position_worldspace;


uniform mat4 ModelViewProjection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
	Position_worldspace=(ModelMatrix*vec4(position,1)).xyz;
	
	vec3 vertexPosition_cameraspace=(ViewMatrix*ModelMatrix*vec4(position,1)).xyz;
	

	position0=(ModelMatrix*vec4(position,0.)).xyz;
	texCoord0=texCoord;
	normal0=(transpose(inverse(ModelMatrix))*vec4(normal,0.)).xyz;
	gl_Position=ModelViewProjection*vec4(position,1.);
}