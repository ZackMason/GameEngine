#version 460

uniform sampler2D diffuse;
uniform sampler2D norm;

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 Position_worldspace;

uniform vec3 LightPosition;
uniform float Time;
uniform float bias;
uniform float scale;
uniform float power;


out vec4 color;

float fresnel(vec3 rd,vec3 n)
{
	return 0.0+0.47*pow(1.+dot(rd,n),7.0);
}

float calcWave(float x)
{
    float y = (sin(x * 1.0 + Time * 1.0) + sin(x * 2.3 + Time * 1.5) + sin(x * 3.3 + Time * 0.4)) / 3.0;
    return y;
}

float rand(vec2 c){
	return fract(sin(dot(c.xy,vec2(12.9898,78.233)))*43758.5453);
}

float noise(vec2 p,float freq){
	float unit=640./freq;
	vec2 ij=floor(p/unit);
	vec2 xy=mod(p,unit)/unit;
	//xy = 3.*xy*xy-2.*xy*xy*xy;
	xy=.5*(1.-cos(3.141592*xy));
	float a=rand((ij+vec2(0.,0.)));
	float b=rand((ij+vec2(1.,0.)));
	float c=rand((ij+vec2(0.,1.)));
	float d=rand((ij+vec2(1.,1.)));
	float x1=mix(a,b,xy.x);
	float x2=mix(c,d,xy.x);
	return mix(x1,x2,xy.y);
}

void main()
{
	vec3 col2 = vec3 (0.,.0,1.);
	vec3 LightColor = vec3(0.9922, 0.9098, 0.4353);
	float LightPower = 250.0f;
	
	vec2 uv = Position_worldspace.xz/100.;
	uv.y+=calcWave(uv.x)*.21;
	uv.x+=calcWave(uv.y)*.21;
	// Material properties
	vec3 E = normalize(EyeDirection_cameraspace);

	vec3 n = normalize( normal0 );
	n += normalize(texture(norm,uv+Time/18.).rgb)*9.;
	n = normalize(n);
	float a = min(max((Position_worldspace.y-22)/15.,0.),1.);
	a = fresnel(E,n);
	a= max(a,0.);
	a = min(a,1.);

	vec3 MaterialDiffuseColor = mix(vec3(0.2275, 0.2275, 1.0),vec3(0,0,1), a );
	vec3 MaterialSpecularColor = vec3(.3);

	// Distance to the light
	float distance = length( LightPosition - Position_worldspace);
	distance = 10.;
	// Normal of the computed fragment, in camera space
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );
	l = vec3(1,1,0);
	l = normalize(l);
	float dis =  Position_worldspace.y - (35 + noise(Position_worldspace.xz/2., 550.3)*27.);
	MaterialDiffuseColor += smoothstep(0.,2.1,dis)* vec3(0.7608, 0.9765, 0.9922);


	MaterialDiffuseColor += texture(diffuse,uv).rgb/3.;

	vec3 MaterialAmbientColor = vec3(0.) * MaterialDiffuseColor;

	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	color.a = 1.79;
	color.rgb = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
	//color.rgb += fresnel(E,n)/6.;
	//color = vec4(vec3(a),1.);
	//color.rgb = MaterialDiffuseColor;
#if 0
	color.rgb = MaterialDiffuseColor;
	
#endif
}