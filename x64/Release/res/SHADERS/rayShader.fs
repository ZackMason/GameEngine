#version 460

layout(location=0)uniform sampler2D diffuse;
layout(location=1)uniform sampler2D depth;
uniform float Time;
uniform vec4 ucolor;

in vec2 iuv;

out vec4 color;

const vec2 res=vec2(1920,1080);

#define MAXSTEPS 100
#define MINDIST.01

uniform float EPSILON;
uniform float bias;
uniform float scale;
uniform float power;

vec3 hash3(float n){return fract(sin(vec3(n,n+1.,n+2.))*43758.5453123);}
float mod289(float x){return x-floor(x*(1./289.))*289.;}
vec4 mod289(vec4 x){return x-floor(x*(1./289.))*289.;}
vec4 perm(vec4 x){return mod289(((x*34.)+1.)*x);}

float rand(vec2 n){
	return fract(sin(dot(n,vec2(12.9898,4.1414)))*43758.5453);
}

float noise2(vec2 p){
	vec2 ip=floor(p);
	vec2 u=fract(p);
	u=u*u*(3.-2.*u);
	
	float res=mix(
		mix(rand(ip),rand(ip+vec2(1.,0.)),u.x),
		mix(rand(ip+vec2(0.,1.)),rand(ip+vec2(1.,1.)),u.x),u.y);
		return res*res;
	}

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

float sphere(vec3 pos,vec4 sph){
	return distance(mod(pos,2.),sph.xyz)-sph.w;
}

float terrain(vec3 pos)
{
	//return pos.y;
	return distance(pos,vec3(pos.x,noise2(pos.xz*.5),pos.z));
}

float scene(vec3 pos)
{
	vec4 sph=vec4(1,1,1,.5+abs(noise(pos*3.+vec3(Time*0))-.5)/2.);
	return terrain(pos);
	//return sphere(pos,sph);
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
	
	float trace(vec3 from,vec3 direction){
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
		return totalDistance;
		//return 1.-totalDistance*float(steps)/float(MAXSTEPS);
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
	
	float fresnel(vec3 rd,vec3 n)
	{
		return bias+scale*pow(1.+dot(rd,n),power);
	}
	
	void main()
	{
		vec2 uv=(iuv*res.xy)/res.y-(vec2(.5)*res.xy)/res.y;
		
		vec3 ro=vec3(1,2,-3);
		//vec3 ro=vec3(0,3,0)+(1.*vec3(0,-.5*Time,Time));
		vec3 rd=normalize(vec3(uv.x,uv.y-.25,1));
		rd=(rotateY(Time/10.)*vec4(rd,1)).xyz;
		float dist=trace(ro,rd);
		vec3 p=ro+(rd*dist);
		
		vec3 n=(estimateNormal(p));
		
		//color.rgb = vec3(dist) * dot(n,vec3(0,1,0));
		color.rgb=mix(ucolor.rgb,vec3(0,0,1),noise2(p.xz*1.));
		vec3 icol=1.-color.rgb;
		//color.rgb += icol *vec3(0.098, 0.8784, 0.8157);
		//color.rgb = icol.rrr;
		//color.rgb *= clamp(50.-dist,0,1);
		
		//color.rgb = color.rgb/(1+color.rgb);
		
		//dithering
		color.rgb+=(1./255.)*hash3(uv.x+13.*uv.y);
		color.a=1;
	}