#version 460

uniform sampler2D diffuse;
uniform sampler2D norm;

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 Position_worldspace;
in mat3 TBN;

uniform vec3 LightPosition;
uniform float Time;
uniform float bias;
uniform float scale;
uniform float power;
uniform float shine;
uniform float lp;
uniform vec4 ucolor;

layout (location = 0 )out vec4 color;
layout (location = 1 )out vec4 dcolor;


vec3 FuzzyShading(vec3 c, vec3 n, vec3 e, float core, float power, float edge)
{
	float d = dot (e,n);
	d = clamp(d,0.,1.);
	float d1 = pow((1-d),power) * edge;
	float d2 = (1-(d*core));
	return (d1+d2)*c;
}


float near=.1;
float far=150.;

float LinearizeDepth(float depth)
{
	float z=depth*2.-1.;// back to NDC
	return(2.*near*far)/(far+near-z*(far-near));
}

float fresnel(vec3 rd,vec3 n)
{
	return 0.+.11*pow(1.+dot(rd,n),2.6);
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

vec4 hash4(vec2 p){return fract(sin(vec4(1.+dot(p,vec2(37.,17.)),
	2.+dot(p,vec2(11.,47.)),
	3.+dot(p,vec2(41.,29.)),
4.+dot(p,vec2(23.,31.))))*103.);}


//http://www.iquilezles.org/www/articles/texturerepetition/texturerepetition.htm
vec4 textureNoTile( sampler2D samp, in vec2 uv )
{
    vec2 p = floor( uv );
    vec2 f = fract( uv );
	
    // derivatives (for correct mipmapping)
    vec2 ddx = dFdx( uv );
    vec2 ddy = dFdy( uv );
    
    // voronoi contribution
    vec4 va = vec4( 0.0 );
    float wt = 0.0;
    for( int j=-1; j<=1; j++ )
    for( int i=-1; i<=1; i++ )
    {
        vec2 g = vec2( float(i), float(j) );
        vec4 o = hash4( p + g );
        vec2 r = g - f + o.xy;
        float d = dot(r,r);
        float w = exp(-5.0*d );
        vec4 c = textureGrad( samp, uv + o.zw, ddx, ddy );
        va += w*c;
        wt += w;
    }
	
    // normalization
    return va/wt;
}

void main()
{
	vec3 col2 = vec3 (0.,.0,1.);
	vec3 LightColor = vec3(0.9922, 0.9098, 0.4353);
	float LightPower = 500.;
	
	vec2 uv = Position_worldspace.xz/70.;
	uv.y+=calcWave(uv.x)*.21;
	uv.x+=calcWave(uv.y)*.21;
	// Material properties
	vec3 E = normalize(EyeDirection_cameraspace);

	vec3 n = normalize( normal0 );
	n = normalize(mix(textureNoTile(norm,uv.yx+Time/8.).rgb*2.-1.,textureNoTile(norm,uv-vec2(Time,-Time)/9.).rgb*2.-1.,.5));
	n = TBN * normalize(n);
	float a = min(max((Position_worldspace.y-22)/15.,0.),1.);
	a = fresnel(E,n);
	a= max(a,0.);
	a = min(a,1.);

	//vec3 MaterialDiffuseColor = mix(vec3(0,0,1),vec3(0.2275, 0.2275, 1.0), a );
	//vec3 MaterialDiffuseColor = FuzzyShading(vec3(0,0,1),n,E,0.496,7.,.225);
	//vec3 MaterialDiffuseColor = FuzzyShading(vec3(.3373,.3373,.9451),n,E,scale,power,bias);
	vec3 MaterialDiffuseColor = FuzzyShading(ucolor.rgb,n,E,scale,power,bias);
	vec3 MaterialSpecularColor = vec3(.9);

	// Distance to the light
	float distance = length( LightPosition - Position_worldspace);
	distance = shine;
	// Normal of the computed fragment, in camera space
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );
	l = vec3(1,1,0);
	l = normalize(l);
	float dis =  Position_worldspace.y - (35 + noise(Position_worldspace.xz/.3, 550.3)*(27.));
	MaterialDiffuseColor += smoothstep(0.,4.1,dis/4.)* vec3(0.7608, 0.9765, 0.9922);


	//MaterialDiffuseColor += textureNoTile(diffuse,uv).rgb/3.;

	vec3 MaterialAmbientColor = vec3(0.7) * MaterialDiffuseColor;

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
	cosAlpha = smoothstep(.7,1.,cosAlpha);
	color.a = 1.;
	color.rgb = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
	//color.rgb = fresnel(E,n)*vec3(1);
	//color = vec4(vec3(a),1.);

    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    dcolor = vec4(vec3(depth), 1.0);

#if 0
	color.rgb = MaterialDiffuseColor;
	
#endif
}