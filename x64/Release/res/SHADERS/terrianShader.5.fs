#version 460

uniform sampler2D diffuse;

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 Position_worldspace;
in vec4 viewspace;

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

float rand2(vec2 c){
    return fract(sin(dot(c.xy,vec2(12.9898,78.233)))*43758.5453);
}

float noise2(vec2 p,float freq){
    float unit=640./freq;
    vec2 ij=floor(p/unit);
    vec2 xy=mod(p,unit)/unit;
    //xy = 3.*xy*xy-2.*xy*xy*xy;
    xy=.5*(1.-cos(3.141592*xy));
    float a=rand2((ij+vec2(0.,0.)));
    float b=rand2((ij+vec2(1.,0.)));
    float c=rand2((ij+vec2(0.,1.)));
    float d=rand2((ij+vec2(1.,1.)));
    float x1=mix(a,b,xy.x);
    float x2=mix(c,d,xy.x);
    return mix(x1,x2,xy.y);
}

float pnoise(vec2 p){
    int res=4;
    float persistance=.5;
    float n=0.;
    float normK=0.;
    float f=4.;
    float amp=1.;
    int iCount=0;
    for(int i=0;i<50;i++){
        n+=amp*noise2(p,f);
        f*=2.;
        normK+=amp;
        amp*=persistance;
        if(iCount==res)break;
        iCount++;
    }
    float nf=n/normK;
    return nf*nf*nf*nf;
}

float h(vec2 p)
{
    float xz_scale=6.;
    
    float freq=1;
    float amp=1;
    float he=0;
    float lac=2.2312;
    float pers=.12;
    
    int octaves=1;
    
    //Position_worldspace.z-=Time*3500;
    for(int i=0;i<octaves;i++)
    {
        float sx=p.x/xz_scale*freq;
        float sz=p.y/xz_scale*freq;
        float pv=2*pnoise(vec2(sx,sz))-1;
        he+=pv*amp;
        amp*=pers;
        freq*=lac;
    }
    return he;
}

float grid(vec3 st,float res)
{
    vec3 grid=fract(st*res);
    return(step(res,grid.x)*step(res,grid.y)*step(res,grid.z));
}

float gridf(float st,float res)
{
    float grid=fract(st*res);
    return(step(res,grid));
}

void main()
{
    vec3 LightColor=vec3(1);
    vec3 col2=vec3(0.,.0,1.);
    LightColor=mix(LightColor,col2,abs(sin(Time)));
    LightColor=vec3(1);
    float LightPower=23.f;
    
    // Material properties
    vec3 MaterialDiffuseColor;
    vec3 MaterialSpecularColor=vec3(.3,.3,.3);
    
    vec3 c1 = vec3(0.0549, 0.5529, 0.0549);
    vec3 c2 = vec3(0.2431, 0.2667, 0.0196);
    vec3 c3 = vec3(0.5569, 0.8667, 0.349);
    vec3 c4 = vec3(0.149, 0.2431, 0.99686);
    // Distance to the light
    vec2 p0 = Position_worldspace.xz;
    vec2 p1 = Position_worldspace.xz;
    p1.y += 1.;
    
    vec2 p2 = Position_worldspace.xz;
    p2.x += 1.;
    float y_scale = 400;    
    vec3 pp0;
    pp0.x = p0.x;
    pp0.z = p0.y;
    pp0.y = abs(h(p0))*y_scale;//*2.+580;
    pp0.y -= 200;
    vec3 pp1;
    pp1.x = p1.x;
    pp1.z = p1.y;
    //pp1.y = h(p1)*y_scale*2.+580;
    pp1.y = abs(h(p1))*y_scale;//*2.+580;
    pp1.y-=200;
    vec3 pp2;
    pp2.x = p2.x;
    pp2.z = p2.y;
    pp2.y = abs(h(p2))*y_scale;//*2.+580;
    pp2.y-=200;
    
    //pp2.y = h(p2)*y_scale*2.+580;
    if (pp0.y < 35)
        pp0.y = 35;
    if (pp1.y < 35)
        pp1.y = 35;
    if (pp2.y < 35)
        pp2.y = 35;
    vec3 a = pp0 - pp1;
    vec3 b = pp0 - pp2;
    vec3 n = cross(a,b);
    n=normalize(n);

    if    (position0.y <20)
    {
        MaterialDiffuseColor=c4*(.73+(noise(Position_worldspace.xz*0.1)*0.1));
    }
    else if(position0.y<30)
    {
        MaterialDiffuseColor=c4*.9;
    }
    else if(position0.y+noise(Position_worldspace.xz+Time)*2<35)
    {
        MaterialDiffuseColor = c4;
    }
    else if(position0.y<35)
    {
        MaterialDiffuseColor = vec3(1);
    }
    else if(position0.y+noise(Position_worldspace.xz)*2<40)
    {
        MaterialDiffuseColor=c3;
    }
    else if(position0.y+(noise(Position_worldspace.xz)*25)+noise(Position_worldspace.xz/100)*63<170)
    //else if(position0.y+(noise(Position_worldspace.xz) * 68)+noise(-Position_worldspace.xz/100)*163 <340)
    {
        //c1 =mix(c1,c2,(noise(Position_worldspace.xz)*.4));
        MaterialDiffuseColor = c1;
        //c2=c1*.3  +(noise(Position_worldspace.xz)*.1);
        //MaterialDiffuseColor = mix(c2,c1,dot(n,vec3(0,1,0))*dot(n,vec3(0,1,0))*dot(n,vec3(0,1,0))*dot(n,vec3(0,1,0)));
        //MaterialDiffuseColor = texture(diffuse, Position_worldspace.xz/10.).rgb * c1;
    //}
    //{
    }
    else if (position0.y < 340)
    {
        MaterialDiffuseColor = c2;

    }
    else
    {
        

        //if (dot(n,vec3(0,1,0)) < 0.6)
          //  MaterialDiffuseColor=c1*.3  +(noise(Position_worldspace.xz)*.1);
        //else
        MaterialDiffuseColor = vec3(0.8);// * dot(n,vec3(0,1,0));
        //MaterialDiffuseColor = n;   
    }

#if 0
        
    
    //MaterialDiffuseColor = vec3(dot(n,vec3(0,1,1)));


    float dist=0;
    dist = abs(viewspace.z);

    float fogend = 8000.;

    float fogFactor = (fogend - dist)/(fogend - 20.);
   fogFactor = clamp( fogFactor, 0.0, 1.0 );
 
   //if you inverse color in glsl mix function you have to
   //put 1.0 - fogFactor
    //MaterialDiffuseColor *= finalColor;
    //MaterialDiffuseColor =  mix(c2,c1,(position0.y+120)/50.);
    //  MaterialDiffuseColor += vec3(0,1,0) * noise(Position_worldspace.xz) *0.1;
    float distance = 3.;
    vec3 l = vec3(1,1,0);
    l = normalize(l);
    float cosTheta=clamp(dot(n,l),0,1);
    vec3 R=reflect(-l,n);
    vec3 E=normalize(EyeDirection_cameraspace);
    float cosAlpha=clamp(dot(E,R),0,1);
    MaterialDiffuseColor += MaterialDiffuseColor *0.1;
    MaterialDiffuseColor=(MaterialDiffuseColor*.31) + (MaterialDiffuseColor*LightColor*LightPower*cosTheta/(distance*distance));
#endif

    #if 0
    float distance=length(LightPosition-Position_worldspace);
    
    vec3 MaterialAmbientColor=vec3(.21,.21,.21)*MaterialDiffuseColor;
    // Normal of the computed fragment, in camera space
    
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
    MaterialDiffuseColor = finalColor;
    color.rgb=
    // Ambient : simulates indirect lighting
    MaterialAmbientColor+
    // Diffuse : "color" of the object
    MaterialDiffuseColor*LightColor*LightPower*cosTheta/(distance*distance)+
    // Specular : reflective highlight, like a mirror
    MaterialSpecularColor*LightColor*LightPower*pow(cosAlpha,5)/(distance*distance);
    #endif
    //vec3 finalColor = mix(vec3(0.8), MaterialDiffuseColor, fogFactor);
    float ex = 1.2;
   // color.rgb = finalColor;
    //color.rgb = color.xyz / (color.xyz + vec3(1.0));
    //color.rgb = pow(color.xyz, vec3(1.0/2.2));
	//color.rgb=ex-exp(-ex*color.rgb);
    //color.rgb*=vec3(grid(vec3(Position_worldspace.x,position0.y,Position_worldspace.z),.1));
    //color.rgb = vec3(grid(Position_worldspace,.1));
    //color.r = gridf(Position_worldspace.x,0.1);
    //color.g = gridf(position0.y-35, 0.1);
    //color.b = gridf(Position_worldspace.z,0.1);
    //color.rgb *= vec3(abs(h(Position_worldspace.xz)));
    color.rgb =MaterialDiffuseColor;
    color.rgb = color.rgb / vec3(1.) + color.rgb;
    color.a=1;
}