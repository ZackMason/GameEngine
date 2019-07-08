#version 460

uniform sampler2D diffuse;

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 Position_worldspace;

uniform vec3 LightPosition;
uniform float Time;

out vec4 color;

float rand(vec2 n){
    return fract(sin(dot(n,vec2(12.9898,4.1414)))*43758.5453);
}

float noise(vec2 p){
    vec2 ip=floor(p);
    vec2 u=fract(p);
    u=u*u*(3.-2.*u);
    
    float res=mix(
        mix(rand(ip),rand(ip+vec2(1.,0.)),u.x),
        mix(rand(ip+vec2(0.,1.)),rand(ip+vec2(1.,1.)),u.x),u.y);
        return res*res;
    }

void main()
{
    vec3 LightColor=vec3(1,.1,.1);
    vec3 col2=vec3(0.,.0,1.);
    LightColor=mix(LightColor,col2,abs(sin(Time)));
    LightColor=vec3(1);
    float LightPower=50.f;
    
    // Material properties
    vec3 MaterialDiffuseColor=texture(diffuse,texCoord0).rgb;
    vec3 MaterialSpecularColor=vec3(.3,.3,.3);
    
    vec3 c1 = vec3(0.0549, 0.5529, 0.0549);
    vec3 c2 = vec3(0.2431, 0.2667, 0.0196);
    vec3 c3 = vec3(0.5569, 0.8667, 0.349);
    vec3 c4 = vec3(0.149, 0.2431, 0.99686);
    // Distance to the light

    if    (position0.y <20)
    {
        MaterialDiffuseColor=c4*(.43+(noise(Position_worldspace.xz)*0.1));
    }
    else if(position0.y<30)
    {
        MaterialDiffuseColor=c4*.7;
    }
    else if(position0.y<35)
    {
        MaterialDiffuseColor = c4;
    }
    else if(position0.y<40)
    {
        MaterialDiffuseColor=c3;
    }
    else if(position0.y<70)
    {
        MaterialDiffuseColor=mix(c1,c2,(noise(Position_worldspace.xz)*.4));
    }
    else if(position0.y<90)
    {
        MaterialDiffuseColor=c1*.6+(noise(Position_worldspace.xz)*.1);
    }
    else
    {
        MaterialDiffuseColor = vec3(0.8);
    }

    //MaterialDiffuseColor =  mix(c2,c1,(position0.y+120)/50.);
    //  MaterialDiffuseColor += vec3(0,1,0) * noise(Position_worldspace.xz) *0.1;
    float distance=length(LightPosition-Position_worldspace);
    
    vec3 MaterialAmbientColor=vec3(.21,.21,.21)*MaterialDiffuseColor;
    // Normal of the computed fragment, in camera space
    vec3 n=normalize(normal0);
    // Direction of the light (from the fragment to the light)
    vec3 l=normalize(LightDirection_cameraspace);
    distance = 5;
    // Cosine of the angle between the normal and the light direction,
    // clamped above 0
    //  - light is at the vertical of the triangle -> 1
    //  - light is perpendicular to the triangle -> 0
    //  - light is behind the triangle -> 0
    float cosTheta=clamp(dot(n,l),0,1);
    
    // Eye vector (towards the camera)
    vec3 E=normalize(EyeDirection_cameraspace);
    // Direction in which the triangle reflects the light
    vec3 R=reflect(-l,n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha=clamp(dot(E,R),0,1);
    color.a=1;
    color.rgb=
    // Ambient : simulates indirect lighting
    MaterialAmbientColor+
    // Diffuse : "color" of the object
    MaterialDiffuseColor*LightColor*LightPower*cosTheta/(distance*distance)+
    // Specular : reflective highlight, like a mirror
    MaterialSpecularColor*LightColor*LightPower*pow(cosAlpha,5)/(distance*distance);
    color.rgb= MaterialDiffuseColor;
}