#version 460

#define MAX_STEPS 1000
#define MAX_DIST 1000.
#define SURF_DIST .01


uniform sampler2D diffuse;

in vec2 texCoord0;
in vec3 normal0;
in vec4 color0;
in vec3 EyeDirection_cameraspace;
in vec3 Position_worldspace;
in vec3 vertexPosition_cameraspace;

out vec4 color;

float pMod1(inout float p, float size) {
    float halfsize = size*0.5;
    float c = floor((p + halfsize)/size);
    p = mod(p + halfsize, size) - halfsize;
    return c;
}

float Circle(vec2 uv, vec2 p, float r, float blur)
{
    float d = length(uv-p);
    
    return smoothstep(r,r-blur,d);
}

float GetDist(vec3 p)
{
    float pd = p.y;
    vec3 bp = p;

    vec3 sp = p;
    sp -= vec3(0,1,0);
	float sd = length(sp)-1.;

   	float d = min(sd,pd);
    //d = min(pMod1(sd, 2.), d);
    return d;
}
float RayMarch(vec3 ro, vec3 rd)
{
    float dO=0.;
    
    for (int i=0; i < MAX_STEPS; i++)
    {
        vec3 p = ro + rd * dO;
        float ds = GetDist(p);
        dO += ds;
        if (dO>MAX_DIST || ds<SURF_DIST) break;
    }
    return dO;
}

vec3 GetNormal(vec3 p)
{
    float d = GetDist(p);
    vec2 e = vec2(.01,0);
    
    vec3 n = d - vec3(
        GetDist(p-e.xyy),
        GetDist(p-e.yxy),
        GetDist(p-e.yyx));
    return normalize(n);
}

float GetLight(vec3 p)
{
    vec3 lightPos = vec3(0,55,-2);
    vec3 l = normalize(lightPos-p);
    vec3 n = GetNormal(p);
    
    float dif = clamp(dot(n, l),0.,1.);
    
    float d = RayMarch(p+n*SURF_DIST*2.,l);
    if (d<length(lightPos-p)) dif *=.1;
    
    return dif;
}

void main()
{

	vec2 uv = texCoord0;

	uv -= .5;

	vec3 ro = -normalize(EyeDirection_cameraspace) * 50.;
    //ro.z = 2.;
    vec3 rd = normalize(Position_worldspace + normalize(EyeDirection_cameraspace) * 40.);

	//vec3 rd = -normalize(vec3((vertexPosition_cameraspace - Position_worldspace)));
	//rd = normalize(Position_worldspace - vec3(0,.5,0));

    float d = RayMarch(ro,rd);

	vec3 p = ro + rd * d;

	float dif = GetLight(p);
	vec3 col = vec3(dif);
	vec3 lc = vec3(.6,.8233,.8);

	//gl_FragColor = vec4(col*lc,1.0);
    color = vec4(col*lc,1.0);

	//gl_FragColor = texture2D(diffuse, texCoord0);
}