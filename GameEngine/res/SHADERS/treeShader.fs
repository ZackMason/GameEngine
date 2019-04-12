#version 460

uniform sampler2D diffuse;

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 Position_worldspace;
in vec3 world_normal;
in float distToOrigin;

uniform vec3 LightPosition;
uniform float Time;

out vec4 color;

float map(float value,float inMin,float inMax,float outMin,float outMax){
	return outMin+(outMax-outMin)*(value-inMin)/(inMax-inMin);
}

void main()
{
	vec3 LightColor = vec3(1,0.1,0.1);
	vec3 col2 = vec3 (0.,.0,1.);
	LightColor = mix(LightColor,col2, abs(sin(Time)));
	LightColor = vec3(1);
	float LightPower = 50.0f;
	
	// Material properties
	vec3 MaterialDiffuseColor = texture( diffuse, texCoord0 ).rgb;
	vec3 MaterialAmbientColor = vec3(0.21,0.21,0.21) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	// Distance to the light
	float distance = length( LightPosition - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( world_normal );
	// Direction of the light (from the fragment to the light)
	vec3 ld = vec3(.2,-.5,.7);
	distance = 10.;
	
	vec3 l = normalize( ld );
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
	
	if(dot((world_normal).xyz,vec3(0.,1.,0.))>-0.2&&position0.y>17)
	{
		MaterialAmbientColor.rgb = mix(vec3(1.2), MaterialAmbientColor, clamp(dot((world_normal).xyz,vec3(0.,1.,0.)),0.,1.));
	}
	if(dot((world_normal).xyz,vec3(0.,1.,0.))>-0.43&&position0.y>17)
	{
		MaterialAmbientColor.rgb = mix(vec3(.9), MaterialAmbientColor, 0.5);
	}
	
	color.rgb = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
	color.rgb = color.rgb * 1.-map(distToOrigin,0.,75.,-0.3250,0.7) ;
}

