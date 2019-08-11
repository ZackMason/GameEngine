#version 460

struct Material{
    sampler2D diffuse;
    sampler2D normal;
    sampler2D specular;
    sampler2D roughness;
    sampler2D metallic;
    float shininess;
};

uniform Material material;

in vec2 texCoord0;
in vec3 normal0;
in vec3 Position_worldspace;

layout(location=0)out vec4 o_pos;
layout(location=1)out vec4 o_norm;
layout(location=2)out vec4 o_color;

void main()
{
    o_pos.rgb=Position_worldspace;
    o_color = texture(material.diffuse,texCoord0);
    //o_color.rgb=vec3(1.,0.,.0);
    o_norm = texture(material.normal, texCoord0) * 2. - 1.;
    o_norm.xyz=normalize(normal0);
    o_color.a = .3; // texture(material.specular,texCoord0).r;
    //o_pos.a = texture(material.metallic,texCoord0).r;
    o_pos.a = 1.;
    o_norm.a = texture(material.roughness, texCoord0).r *.0 + .2;
    o_norm.a = .4;
}