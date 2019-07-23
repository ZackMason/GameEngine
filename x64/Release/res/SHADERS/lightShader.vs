#version 460

layout(location=0)in vec3 position;
layout(location=1)in vec2 texCoord;
layout(location=2)in vec3 normal;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 vd;

uniform mat4 ModelViewProjection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
	FragPos=vec3(ModelMatrix*vec4(position,1.));
	vec3 vertexPosition_cameraspace = ( ViewMatrix * ModelMatrix * vec4(position,1)).xyz;
	vd = vec3(0,0,0) - vertexPosition_cameraspace;
	Normal=mat3(transpose(inverse(ModelMatrix)))*normal;
	//Normal=(ModelMatrix*vec4(normal,0.)).xyz;
	TexCoords=texCoord;
	
	gl_Position=ModelViewProjection*vec4(position,1.);
}