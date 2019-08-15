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

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 dcolor;

float hash( float n )
{
  return fract(cos(n)*41415.92653);
}

// 2d noise function
float noise( in vec2 x )
{
  vec2 p  = floor(x);
  vec2 f  = smoothstep(0.0, 1.0, fract(x));
  float n = p.x + p.y*57.0;

  return mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
    mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y);
}

float noise( in vec3 x )
{
  vec3 p  = floor(x);
  vec3 f  = smoothstep(0.0, 1.0, fract(x));
  float n = p.x + p.y*57.0 + 113.0*p.z;

  return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
    mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
    mix(mix( hash(n+113.0), hash(n+114.0),f.x),
    mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
}

mat3 m = mat3( 0.00,  1.60,  1.20, -1.60,  0.72, -0.96, -1.20, -0.96,  1.28 );

// Fractional Brownian motion
float fbmslow( vec3 p )
{
  float f = 0.5000*noise( p ); p = m*p*1.2;
  f += 0.2500*noise( p ); p = m*p*1.3;
  f += 0.1666*noise( p ); p = m*p*1.4;
  f += 0.0834*noise( p ); p = m*p*1.84;
  return f;
}

float fbm( vec3 p )
{
  float f = 0., a = 1., s=0.;
  f += a*noise( p ); p = m*p*1.149; s += a; a *= .75;
  f += a*noise( p ); p = m*p*1.41; s += a; a *= .75;
  f += a*noise( p ); p = m*p*1.51; s += a; a *= .65;
  f += a*noise( p ); p = m*p*1.21; s += a; a *= .35;
  f += a*noise( p ); p = m*p*1.41; s += a; a *= .75;
  f += a*noise( p ); 
  return f/s;
}

void main()
{
	vec3 LightColor = vec3(1);
	float LightPower = 50.0f;
	// Material properties
	//vec3 MaterialDiffuseColor = texture( diffuse, texCoord0 ).rgb;
	vec3 hor = vec3(1.5);

	vec2 moon = vec2 (.50,.7);
	moon = texCoord0 - moon;
	

	vec3 c1 = pow(fbmslow(-position0/500.),3.) * vec3(0.8588, 0.4941, 0.0784);
	vec3 c2 = pow(fbmslow((Position_worldspace+ vec3(555))/500.),5.)* vec3(0.8392, 0.9922, 0.1647);
	c1 = mix(c2,c1,fbm((position0+ vec3(555))/500.));
	c1 = smoothstep(0.,.6,c1);
	vec3 sky = pow(fbmslow(position0/500.),2) * vec3(0.1451, 0.549, 0.9255);

	sky = mix(sky,c1,.7-pow(fbmslow(position0/500.),2));

	vec3 MaterialDiffuseColor = mix(hor, sky, min(Position_worldspace.y/600.,1.0)); 
	MaterialDiffuseColor = mix(MaterialDiffuseColor,MaterialDiffuseColor*.4,min(Position_worldspace.y/4500.,1.0));
	vec3 MaterialAmbientColor = vec3(0.21,0.21,0.21) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);
	
	float n1 = noise(texCoord0*200.);


	if(min((n1*.5)+.5,1.)>0.99 && Position_worldspace.y > 2000)
	{
		MaterialDiffuseColor = vec3(1);
	}

	if (length(moon) < .1)
		MaterialDiffuseColor = vec3(1);


	// Distance to the light
	#if 0
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
	color.rgb = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
	#endif
	color.a = 1;
	color.rgb = MaterialDiffuseColor + MaterialAmbientColor * 8.;
	dcolor = vec4(1);
	//color.rgb=1.-exp(-1.*color.rgb);
}