#version 460

struct Material{
    sampler2D diffuse;
    sampler2D normal;
    sampler2D specular;
    float shininess;
};

uniform Material material;
uniform float Time;

in vec2 texCoord0;
in vec3 normal0;
in vec3 vd;
in vec3 Position_worldspace;

in mat3 TBN;

layout(location=0)out vec4 o_pos;
layout(location=1)out vec4 o_norm;
layout(location=2)out vec4 o_color;

vec3 fresnelSchlick(float cosTheta,vec3 F0);

float calcWave(float x)
{
    float y = (sin(x * 1.0 + Time * 1.0) + sin(x * 2.3 + Time * 1.5) + sin(x * 3.3 + Time * 0.4)) / 3.0;
    return y;
}


#define MAX_MARCH_STEPS 16
#define MARCH_STEP_SIZE 0.2

#define NOISE_AMPLITUDE 0.75

#define FBM_ITERATIONS 3
#define FBM_AMPLITUDE_GAIN 0.8
#define FBM_FREQUENCY_GAIN 1.9
vec3 Hash3( vec3 p ) { return fract(sin(vec3( dot(p,vec3(127.1,311.7,786.6)), dot(p,vec3(269.5,183.3,455.8)), dot(p,vec3(419.2,371.9,948.6))))*43758.5453); }
float Voronoi(vec3 p)
{
	vec3 n = floor(p);
	vec3 f = fract(p);

	float shortestDistance = 1.0;
	for (int x = -1; x < 1; x++) {
		for (int y = -1; y < 1; y++) {
			for (int z = -1; z < 1; z++) {
				vec3 o = vec3(x,y,z);
				vec3 r = (o - f) + 1.0 + sin(Hash3(n + o)*50.0)*0.2;
				float d = dot(r,r);
				if (d < shortestDistance) {
					shortestDistance = d;
				}
			}
		}
	}
	return shortestDistance;
}

float FractalVoronoi(vec3 p)
{
	float n = 0.0;
	float f = 0.5, a = 0.5;
	mat2 m = mat2(0.8, 0.6, -0.6, 0.8);
	for (int i = 0; i < FBM_ITERATIONS; i++) {
		n += Voronoi(p * f) * a;
		f *= FBM_FREQUENCY_GAIN;
		a *= FBM_AMPLITUDE_GAIN;
		p.xy = m * p.xy;
	}
	return n;
}

// 2D Random
float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners percentages
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

void main()
{
    o_pos.rgb=Position_worldspace;
    o_color.rgb=vec3(0.0, 0.4667, 1.0);
    o_color.rgb=mix(vec3(0.4353, 0.7451, 1.0) ,vec3(0.0157, 0.0, 1.0) ,1.- pow(Voronoi((o_pos.rgb+Time)/25.) ,7.)*6.*noise(o_pos.xz/2.) );
    vec2 uv = o_pos.xz/4.;
    uv.x += calcWave(uv.y)/4.;
    o_norm.rgb = mix( vec3(0,0,1), texture(material.normal, uv).rgb * 2. - 1., 0.0);
    o_norm.xyz=normalize(TBN*o_norm.rgb);
    //o_color.rgb = fresnelSchlick(max(0.,1.-dot(o_norm.xyz, vd)),o_color.rgb);
    o_color.a = .3; // texture(material.specular,texCoord0).r;
    o_pos.a = .2;
    o_norm.a = .1;
}

vec3 fresnelSchlick(float cosTheta,vec3 F0)
{
    return F0+(1.-F0)*pow(1.-cosTheta,5.);
}