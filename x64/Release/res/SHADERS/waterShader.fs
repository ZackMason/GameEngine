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


float calcWave(float x)
{
    float y = (sin(x * 1.0 + Time * 1.0) + sin(x * 2.3 + Time * 1.5) + sin(x * 3.3 + Time * 0.4)) / 3.0;
    return y;
}

void main()
{
	vec3 LightColor = vec3(1,0.1,0.1);
	vec3 col2 = vec3 (0.,.0,1.);
	LightColor = mix(LightColor,col2, abs(sin(Time)));
	LightColor = vec3(1);
	float LightPower = 50.0f;
	
	// Material properties
    vec2 uv = texCoord0 * 1. ;
    //uv.x += Time;
	float y = uv.y;
    uv.y += calcWave(Position_worldspace.x)*.021;
    uv.x += calcWave(Position_worldspace.z)*.021;
	//uv.x += calcWave(y)*.1;

    //uv.y += sin(uv.x);

	vec3 MaterialDiffuseColor = texture( diffuse, uv ).rgb;
	uv.y += .2;
	uv.x += .2;
	vec3 t = texture(diffuse,uv ).rgb;

	t *= vec3(0.0196, 0.0588, 0.2314);
	t= 1 - t;


    vec3 inver = 1. - MaterialDiffuseColor;

    inver *= vec3(0.2157, 0.0, 1.0);
    MaterialDiffuseColor += inver * t;


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
	color.a = 1.;
	color.rgb = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
	color.rgb = MaterialDiffuseColor;
#if 0
#endif
}