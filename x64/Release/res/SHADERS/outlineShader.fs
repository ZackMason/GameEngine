#version 460

layout (location = 0) uniform sampler2D diffuse;
layout (location = 1) uniform sampler2D depth;

in vec2 uv;

out vec4 color;

float offset=1./600.;

vec2 offsets[9]=vec2[](
    vec2(-offset,offset),// top-left
    vec2(0.f,offset),// top-center
    vec2(offset,offset),// top-right
    vec2(-offset,0.f),// center-left
    vec2(0.f,0.f),// center-center
    vec2(offset,0.f),// center-right
    vec2(-offset,-offset),// bottom-left
    vec2(0.f,-offset),// bottom-center
    vec2(offset,-offset)// bottom-right
);

float edge[9]=float[](
    1,1,1,
    1,-8,1,
    1,1,1
);

float den = 4.;
float blur[9]=float[](
    1./den,2./den,1./den,
    2./den,4./den,2./den,
    1./den,2./den,1./den
);

vec3 run_kernel(float[9] kernel, inout vec3 sampleTex[9])
{
    vec3 res = vec3(0.);
    for(int i=0;i<9;i++)
        sampleTex[i]*=vec3(texture(diffuse,uv.st+offsets[i]));
    for(int i=0;i<9;i++)
    {
        res += sampleTex[i]*kernel[i];
        sampleTex[i]*=kernel[i];
    }
    return res;
}

void main()
{
    
    vec3 sampleTex[9] = vec3[9](1);
    vec3 col1 = run_kernel(edge,sampleTex);
    //offset = 1./300.;
    vec3 col2 = run_kernel(blur,sampleTex);
	float d = texture(depth,uv).r;
    color.rgb =col1 + col2;
    //color.rgb = mix(col1.rgb,texture(diffuse,uv).rgb, smoothstep(0.36,.9,(1-d))).rgb;
    //color.rgb=mix(texture(diffuse,uv).rgb,color.rgb,color.rgb);
    //color.rgb = texture(depth,uv).rgb;
    color.a=1;
    //color.rg = uv;
}