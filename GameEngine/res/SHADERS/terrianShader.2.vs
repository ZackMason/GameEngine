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

float rand(vec2 c){
    return fract(sin(dot(c.xy,vec2(12.9898,78.233)))*43758.5453);
}

float noise(vec2 p,float freq){
    float unit=640./freq;
    vec2 ij=floor(p/unit);
    vec2 xy=mod(p,unit)/unit;
    //xy = 3.*xy*xy-2.*xy*xy*xy;
    xy=.5*(1.-cos(3.141592*xy));
    float a=rand((ij+vec2(0.,0.)));
    float b=rand((ij+vec2(1.,0.)));
    float c=rand((ij+vec2(0.,1.)));
    float d=rand((ij+vec2(1.,1.)));
    float x1=mix(a,b,xy.x);
    float x2=mix(c,d,xy.x);
    return mix(x1,x2,xy.y);
}

float pnoise(vec2 p){
    int res = 4;
    float persistance=.5;
    float n=0.;
    float normK=0.;
    float f=4.;
    float amp=1.;
    int iCount=0;
    for(int i=0;i<50;i++){
        n+=amp*noise(p,f);
        f*=2.;
        normK+=amp;
        amp*=persistance;
        if(iCount==res)break;
        iCount++;
    }
    float nf=n/normK;
    return nf*nf*nf*nf;
}

float calcWave(float x)
{
    float y=(sin(x*1.+(Time*1.)+345)+sin(x*2.8+(Time*1.5)+345)+sin(x*3.9+(Time*.4)+345))/3.;
    return y;
}


void main()
{
    Position_worldspace=(ModelMatrix*vec4(position,1)).xyz;
    
    vec3 vertexPosition_cameraspace=(ViewMatrix*ModelMatrix*vec4(position,1)).xyz;
    EyeDirection_cameraspace=vec3(0,0,0)-vertexPosition_cameraspace;
    
    vec3 LightPosition_cameraspace=(ViewMatrix*vec4(Position_worldspace+vec3(1),1)).xyz;
    LightDirection_cameraspace=LightPosition_cameraspace+EyeDirection_cameraspace;
    
    texCoord0=texCoord;
    normal0=(ViewMatrix*ModelMatrix*vec4(normal,0.)).xyz;

    float y_scale = 100.0;
    float xz_scale = 1.0;
    float freq = 1;
    float amp = 1;
    float h = 0;
    float lac = 2.2;
    float pers = .12;
    vec3 pos = position;

    int octaves = 8;

    Position_worldspace.x+=Time*50;
    for (int i = 0; i < octaves; i++)
    {
        float sx = Position_worldspace.x/ xz_scale * freq;
        float sz = Position_worldspace.z/ xz_scale * freq;
        float pv = 2*pnoise(vec2(sx,sz)) - 1;
        h += pv * amp;
        amp *= pers;
        freq *= lac;
    }
    pos.y = h * y_scale + 130;
    position0=(ModelMatrix*vec4(pos,0.)).xyz;

    if (pos.y < 35)
    {
        pos.y = 35;
    }


    gl_Position=ModelViewProjection*vec4(pos,1.);
}