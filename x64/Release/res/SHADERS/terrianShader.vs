#version 460

in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec4 color;

out vec3 normal0;
out vec2 texCoord0;
out vec3 position0;
out vec3 LightDirection_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 Position_worldspace;

uniform mat4 ModelViewProjection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform vec3 LightPosition;
uniform float Time;

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


float calcWave(float x)
{
    float y=(sin(x*1.+(Time*1.)+345)+sin(x*2.8+(Time*1.5)+345)+sin(x*3.9+(Time*.4)+345))/3.;
    return y;
}


void main()
{
    Position_worldspace=(ModelMatrix*vec4(position,1)).xyz;
    
    float y_scale = 60.0;
    float xz_scale = 60.0;
    vec3 pos = position;
    
    float lacunarity = 2;
    float persistence = 2;
    int octaves = 8;
    Position_worldspace.x+=Time*50;
    for (int i = 1; i < octaves; i++)
    {
        pos.y += noise(vec2(Position_worldspace.x/(xz_scale * lacunarity/i),
        Position_worldspace.z/(xz_scale * lacunarity/i))) * y_scale * persistence/i;
    }
    pos.y = pow(pos.y, .88);
#if 0
    pos.y = ((noise(vec2(Position_worldspace.x/30 ,Position_worldspace.z/30))*30.0) + 
            ((noise(vec2(Position_worldspace.x/15 ,Position_worldspace.z/15))*15.0) * 0.5) +
            ((noise(vec2(Position_worldspace.x/7.5,Position_worldspace.z/7.5))*7.5) *.25));
#endif
    position0=(ModelMatrix*vec4(pos,0.)).xyz;
    if(pos.y<35)
        pos.y=35;

    //Position_worldspace=(ModelMatrix*vec4(pos,1)).xyz;


    vec3 vertexPosition_cameraspace=(ViewMatrix*ModelMatrix*vec4(pos,1)).xyz;
    EyeDirection_cameraspace=vec3(0,0,0)-vertexPosition_cameraspace;
    
    vec3 LightPosition_cameraspace=(ViewMatrix*vec4(Position_worldspace+vec3(1),1)).xyz;
    LightDirection_cameraspace=LightPosition_cameraspace+EyeDirection_cameraspace;
    
    texCoord0=texCoord;
    normal0=(ViewMatrix*ModelMatrix*vec4(normal,0.)).xyz;

    gl_Position=ModelViewProjection*vec4(pos,1.);
}