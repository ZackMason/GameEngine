#version 460

in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec4 color;

out vec3 normal0;
out vec2 texCoord0;
out vec3 view0;
out vec3 position0;
out vec4 fragColor;
smooth out vec4 col0;

uniform mat4 ModelViewProjection;

void main()
{
	vec3 eye = vec3(0, 0, 5);
	view0 = eye-position;
	normal0 = normalize((ModelViewProjection * vec4(normal,0.0)).xyz);

	float bias = .046;
	float scale = 0.8;
	float power = 1.7;
	vec3 I = normalize(view0);
	vec3 N = normal0;
	float r = clamp(bias + scale * pow(1.0 + dot(I, N),power),0,1);

	vec3 col1 = vec3(.0,0.,0.);
	vec3 col2 = vec3(.8,.8,.2);

	col0 = vec4(mix(col1,col2, r),1.);
	fragColor = color;
	position0 = vec3(ModelViewProjection * vec4(position, 1.0));
	texCoord0 = texCoord;
	gl_Position = ModelViewProjection * vec4(position, 1.0);
	//gl_PointSize = r0*100;

}
