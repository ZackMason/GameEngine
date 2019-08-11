#version 460

struct Material{
    sampler2D diffuse;
    //sampler2D normal;
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

void main()
{
    o_pos.rgb=Position_worldspace;
    //o_color=texture(material.diffuse,texCoord0);
    o_color.rgb=vec3(0.,0.,1.0);
    //o_norm.rgb = texture(material.diffuse, texCoord0*4.).rgb * 2. - 1.;
    o_norm.rgb = vec3(0,-1,0);
    o_norm.xyz=normalize(TBN*o_norm.rgb);
    o_color.rgb = fresnelSchlick(max(0.,1.-dot(o_norm.xyz, vd)),o_color.rgb);
    o_color.a = .3; // texture(material.specular,texCoord0).r;
    o_pos.a = 0.;
    o_norm.a = .1;
}

vec3 fresnelSchlick(float cosTheta,vec3 F0)
{
    return F0+(1.-F0)*pow(1.-cosTheta,5.);
}