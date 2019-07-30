#version 460

layout (location = 0) uniform sampler2D diffuse;
layout (location = 1) uniform sampler2D depth;
uniform float Time;
uniform vec4 ucolor;

in vec2 iuv;

out vec4 color;

const vec2 res = vec2(1920,1080);

#define MAXSTEPS 200
#define MINDIST.001

uniform float EPSILON;
uniform float bias;
uniform float scale;
uniform float power;

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise(in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

#define OCTAVES 6
float fbm (in vec2 st) {
    // Initial values
    float value = 0.0;
    float amplitude = .5;
    float frequency = 0.;
    //
    // Loop of octaves
    for (int i = 0; i < OCTAVES; i++) {
        value += amplitude * noise(st);
        st *= 2.;
        amplitude *= .5;
    }
    return value;
}


vec3 hash3(float n){return fract(sin(vec3(n,n+1.,n+2.))*43758.5453123);}
float mod289(float x){return x-floor(x*(1./289.))*289.;}
vec4 mod289(vec4 x){return x-floor(x*(1./289.))*289.;}
vec4 perm(vec4 x){return mod289(((x*34.)+1.)*x);}

float noise(vec3 p){
	vec3 a=floor(p);
	vec3 d=p-a;
	d=d*d*(3.-2.*d);
	
	vec4 b=a.xxyy+vec4(0.,1.,0.,1.);
	vec4 k1=perm(b.xyxy);
	vec4 k2=perm(k1.xyxy+b.zzww);
	
	vec4 c=k2+a.zzzz;
	vec4 k3=perm(c);
	vec4 k4=perm(c+1.);
	
	vec4 o1=fract(k3*(1./41.));
	vec4 o2=fract(k4*(1./41.));
	
	vec4 o3=o2*d.z+o1*(1.-d.z);
	vec2 o4=o3.yw*d.x+o3.xz*(1.-d.x);
	
	return o4.y*d.y+o4.x*(1.-d.y);
}

float displacement(in vec3 p)
{
	return sin(20*p.x)*sin(20*p.y)*sin(20*p.z)*cos(6*p.y);
}

float opSmoothUnion( float d1, float d2, float k ) {
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h); }


float sphere(vec3 pos,vec4 sph){
	return distance(pos,sph.xyz)-sph.w;
}

float opDis2(in vec3 p)
{
	return sin(2.*p.y)*sin(2.*p.x)*sin(2.*p.z);
}

float opDisplace(in vec3 p, float shape)
{
	float d1=shape;
	float d2=displacement(p)/2.;
	return d1+d2;
}

vec3 opRep(in vec3 p,in vec3 c)
{
	vec3 q=mod(p,c);
	return q;
}

float scene(vec3 pos)
{
	vec4 sph = vec4(1,1,1,.75);
	//vec3 p = pos;
	vec3 p = opRep(pos,2.0*vec3(1));
	//return opDisplace(p+vec3(Time/6.),sphere(p,sph))/8.;
	return (sphere(p,sph)+noise(pos*5.+vec3(Time))/2.)/2.;
}

vec3 calcNormal(in vec3 pos)
{
	vec2 e=vec2(1.,-1.)*.5773*.0005;
	return normalize(e.xyy*scene(pos+e.xyy).x+
	e.yyx*scene(pos+e.yyx).x+
	e.yxy*scene(pos+e.yxy).x+
	e.xxx*scene(pos+e.xxx).x);
}

vec3 estimateNormal(vec3 p){
	return normalize(vec3(
			scene(vec3(p.x+EPSILON,p.y,p.z))-scene(vec3(p.x-EPSILON,p.y,p.z)),
			scene(vec3(p.x,p.y+EPSILON,p.z))-scene(vec3(p.x,p.y-EPSILON,p.z)),
			scene(vec3(p.x,p.y,p.z+EPSILON))-scene(vec3(p.x,p.y,p.z-EPSILON))
		));
}

vec2 trace(vec3 from,vec3 direction){
	float totalDistance=0.;
	float steps;
	for(steps=0;steps<MAXSTEPS;steps+=1){
		vec3 p=from+totalDistance*direction;
		float dist=scene(p);
		totalDistance+=dist;
		if(dist<MINDIST)
			break;
	}
	//totalDistance = 1;
	return vec2(totalDistance, float(steps)/float(MAXSTEPS));
}

mat4 rotateY(float theta){
	float c=cos(theta);
	float s=sin(theta);
	
	return mat4(
		vec4  (c ,0,s,0),
		vec4(0 ,1,0,0),
		vec4  (-s,0,c,0),
		vec4(0 ,0,0,1)
	);
	
}


float fresnel(vec3 rd, vec3 n)
{
	return bias+scale*pow(1.+dot(rd,n),power);
}


void main ()
{
	vec2 uv=(iuv*res.xy)/res.y-(vec2(.5)*res.xy)/res.y;

	//vec3 ro=vec3(1,2,-3);
	vec3 ro = vec3(0,3,0)+ (1.*vec3(0,-.5*Time,Time));
    vec3 rd = normalize(vec3(uv.x,uv.y-.25,1));
	rd = (rotateY(Time/10.) * vec4(rd,1)).xyz;
	vec2 tv = trace(ro,rd);
	float dist = tv.x;
	vec3 p = ro + (rd * dist);

	vec3 n = (estimateNormal(p));

	//color.rgb = (1.-fresnel(rd,n)) * mix(ucolor.rgb , vec3(0,0,1) ,(fresnel(rd,n)));
	vec3 icol = 1.-color.rgb;
	color.rgb = vec3(smoothstep(0.0,.53,tv.y))*ucolor.rgb;
	color.rgb += (1.-fresnel(rd,n)) * mix(ucolor.rgb , vec3(0,0,1) ,noise(p*1.));
	//color.rgb = vec3(0.5)+abs(noise(p*3.+vec3(Time*1))-.5)/2.;
	//color.rgb += icol *vec3(0.098, 0.8784, 0.8157);
	//color.rgb = icol.rrr;
	//color.rgb *= clamp(50.-dist,0,1);

	//color.rgb = color.rgb/(1+color.rgb);

	//dithering
	color.rgb += (1./255.)*hash3(uv.x+13.*uv.y);
	color.a = 1;
}