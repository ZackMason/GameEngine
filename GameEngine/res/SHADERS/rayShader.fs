#version 460

layout (location = 0) uniform sampler2D diffuse;
layout (location = 1) uniform sampler2D depth;
uniform float Time;
uniform vec4 ucolor;

in vec2 iuv;

out vec4 color;

const vec2 res = vec2(1920,1080);

#define MAXSTEPS 100
#define MINDIST.01
#define EPSILON .1

float DistanceEstimator(vec3 pos){
	
	// translate
	
	float d1=distance(mod(pos,2.),vec3(1,1,1))-.54321;
	
	return d1;
}

vec3 calcNormal(in vec3 pos)
{
	vec2 e=vec2(1.,-1.)*.5773*.0005;
	return normalize(e.xyy*DistanceEstimator(pos+e.xyy).x+
	e.yyx*DistanceEstimator(pos+e.yyx).x+
	e.yxy*DistanceEstimator(pos+e.yxy).x+
	e.xxx*DistanceEstimator(pos+e.xxx).x);
}

float trace(vec3 from,vec3 direction){
	float totalDistance=0.;
	float steps;
	for(steps=0;steps<MAXSTEPS;steps+=.5){
		vec3 p=from+totalDistance*direction;
		float dist=DistanceEstimator(p);
		totalDistance+=dist;
		if(dist<MINDIST)
			break;
	}
	totalDistance = 1;
	return 1.-totalDistance*float(steps)*1.1/float(MAXSTEPS);
}

mat4 rotateY(float theta){
	float c=cos(theta);
	float s=sin(theta);
	
	return mat4(
		vec4(c,0,s,0),
		vec4(0,1,0,0),
		vec4(-s,0,c,0),
		vec4(0,0,0,1)
	);
	
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


void main ()
{
	vec2 uv=(iuv*res.xy)/res.y-(vec2(.5)*res.xy)/res.y;

	vec3 ro=vec3(0,3,0);
	//vec3 ro = vec3(0,3,0)+ (1.*vec3(0,-.5*Time,Time));
    vec3 rd = normalize(vec3(uv.x,uv.y-.25,1));
	//rd = (rotateY(Time/10.) * vec4(rd,1)).xyz;
	float dist = trace(ro,rd);
	vec3 p = ro + rd * dist;

	color.rgb = vec3(dist) * ucolor.rgb;//dot(estimateNormal(p),rd);
	color.rgb = calcNormal(p);
	//color.rgb *= vec3(0.9255, 0.0039, 0.0039);
	//color.rgb =smoothstep(0.1,0.2, vec3(1)*length(uv-vec2(0)));
	//color.rgb *= mix(vec3(0.051, 0.3333, 0.8588),mix(vec3(0.8784, 0.1373, 0.1373),vec3(0.9529, 0.6745, 0.0235),abs(cos(Time)))
	//,abs(sin(Time)));

	//dithering
	color.rgb += (1./255.)*hash3(uv.x+13.*uv.y);
	color.a = 1;
}