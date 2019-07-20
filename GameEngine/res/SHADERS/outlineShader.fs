#version 460

layout (location = 0) uniform sampler2D diffuse;
layout (location = 1) uniform sampler2D depth;

in vec2 uv;

out vec4 color;

const float offset=1./600.;

void main()
{
    
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
    
    #if 1
    float kernel[9]=float[](
        1, 1, 1,
        1,-8, 1,
        1, 1, 1
    );
    #else
    float kernel[9]=float[](
        1./16,2./16,1./16,
        2./16,4./16,2./16,
        1./16,2./16,1./16
    );
    #endif
    
    vec3 sampleTex[9];
    for(int i=0;i<9;i++)
    {
        sampleTex[i]=vec3(texture(diffuse,uv.st+offsets[i]));
    }
    vec3 col=vec3(0.);
    for(int i=0;i<9;i++)
    col+=sampleTex[i]*kernel[i];
    
	float d = texture(depth,uv).r;
    color.rgb = mix(col.rgb,texture(diffuse,uv).rgb, smoothstep(0.6,.9,(1-d))).rgb;
    //color.rgb=mix(texture(diffuse,uv).rgb,color.rgb,color.rgb);
    //color.rgb = vec3(d);
    color.a=1;
    //color.rg = uv;
}