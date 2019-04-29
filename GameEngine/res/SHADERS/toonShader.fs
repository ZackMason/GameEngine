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

out vec4 color;

void main()
{
	float lit = 0.1;
	float unlit = 0.4;
	vec3 unlitc = vec3(0.5);
	vec3 LightColor = vec3(1,0.1,0.1);
	vec3 col2 = vec3 (0.,.0,1.);
	LightColor = mix(LightColor,col2, abs(sin(Time)));
	LightColor = vec3(1.0, 1.0, 1.0);
	float LightPower = 50.0f;
	
	// Material properties
	vec3 MaterialDiffuseColor = vec3(0.6588, 0.1098, 0.1098);
	vec3 MaterialAmbientColor = vec3(0.21,0.21,0.21) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	// Distance to the light
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

	float atten = 1.;
	float thres = 0.1;

	if ( atten * max(0.0, dot(normal0,l)) >= thres)
	{
		color.rgb = vec3(LightColor) * vec3(MaterialDiffuseColor);
	}

	if (dot(EyeDirection_cameraspace,normal0)
		<
		mix(unlit,lit,max(0.0, dot(normal0,l))))
	{
		color.rgb = vec3(LightColor) * vec3(0.1);
	}

	if (dot(normal0,l) > 0.0 &&
		atten * pow(max(0.0,dot(reflect(-l,normal0),E)),10.0) > 0.5
		)
	{
		color.rgb = vec3(LightColor) * MaterialSpecularColor;
	}
	color.a = 1;
	//color.rgb = 
	//	// Ambient : simulates indirect lighting
	//	MaterialAmbientColor +
	//	// Diffuse : "color" of the object
	//	MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
	//	// Specular : reflective highlight, like a mirror
	//	MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
}