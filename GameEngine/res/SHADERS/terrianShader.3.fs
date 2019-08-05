#version 460
uniform sampler2D diffuse;

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 Position_worldspace;
in vec4 viewspace;

//uniform vec3 LightPosition;
uniform float Time;
uniform float lp;
uniform float shine;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 dcolor;

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

float grid2(vec2 st)
{
    vec2 grid = abs(fract(st-0.5)-0.5)/fwidth(st);
    float line = min(grid.x,grid.y);
    return line;
}

float gridf(float st,float res)
{
    float grid=fract(st*res);
    return(step(res,grid));
}

float near = .1; 
float far  = 150.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
    vec3 LightColor=vec3(0.9529, 0.9333, 0.6353);

    float LightPower=lp;
    
    vec3 c1 = vec3(0.0353, 0.8784, 0.0353);
    // Material properties
    vec3 MaterialDiffuseColor = c1;
    vec3 MaterialSpecularColor=vec3(.3,.3,.3);
    
    vec3 c2 = vec3(0.4745, 0.302, 0.2235);
    vec3 c3 = vec3(0.5569, 0.8667, 0.349);
    vec3 c4 = vec3(0.149, 0.2431, 0.99686);
    // Distance to the light

    vec2 p0 = Position_worldspace.xz;
    vec2 p1 = Position_worldspace.xz;
    vec2 p3 = p0;
    p0.x -= .3;
    p3.y -= .3;
    p1.y += .3;
    vec2 p2 = Position_worldspace.xz;
    p2.x += .3;
    float y_scale = 300;    
    vec3 pp0;
    pp0.x = p0.x;
    pp0.z = p0.y;
    pp0.y = h(p0)*y_scale*2.+580;
    vec3 pp1;
    pp1.x = p1.x;
    pp1.z = p1.y;
    pp1.y = h(p1)*y_scale*2.+580;
    vec3 pp2;
    vec3 pp3;
    pp2.x = p2.x;
    pp2.z = p2.y;
    pp2.y = h(p2)*y_scale*2.+580;
    pp3.x = p3.x;
    pp3.z = p3.y;
    pp3.y = h(p3)*y_scale*2.+580;
    if (pp0.y < 35)
        pp0.y = 35;
    if (pp1.y < 35)
        pp1.y = 35;
    if (pp2.y < 35)
        pp2.y = 35;
    if (pp3.y < 35)
        pp3.y = 35;
    vec3 a = pp0 - pp2;
    vec3 b = pp1 - pp3;
    vec3 n = cross(a,b);
    n=normalize(n);
#if 0
    if(position0.y <10)
    {
        MaterialDiffuseColor=c4*(.73);
    }
    else if(position0.y<20)
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
    #endif
    /*else */if(position0.y+noise(Position_worldspace.xz)*2<40)
    {
        MaterialDiffuseColor=c3;
    }
    else if(position0.y+(noise(Position_worldspace.xz) * 68)+noise(-Position_worldspace.xz/100)*163 <340)
    {
        c2 = c2+ noise(Position_worldspace.xz/10.)*.4;
        c1= c1+(noise(Position_worldspace.xz/100)*.1);
        float l = dot(n,vec3(0,1,0));
        //l = 1 - l*l;
        #if 0
        vec3 proj;
        vec3 ba = abs(n);
        ba /= ba.x + ba.y + ba.z;
        proj.y =texture(diffuse,Position_worldspace.xz/30.).r * ba.y;
        proj.x =texture(diffuse,Position_worldspace.yz/30.).r * ba.x;
        proj.z =texture(diffuse,Position_worldspace.xy/30.).r * ba.z;

        #endif

        //c1 = (proj.x + proj.y + proj.z)/3. * c1;
        MaterialDiffuseColor = mix(c2*.3,c1,l*l*l*l*l*l);
        //MaterialDiffuseColor = c1;
    //}
    //{
    }
    else
    {
        

        //if (dot(n,vec3(0,1,0)) < 0.6)
          //  MaterialDiffuseColor=c1*.3  +(noise(Position_worldspace.xz)*.1);
        //float l=dot(n,vec3(0,1,0));
        //else
        //c1= c1*.13  +(noise(Position_worldspace.xz)*.1);

        //MaterialDiffuseColor=mix(c1,vec3(0.8),l*l*l*l);

        MaterialDiffuseColor = vec3(0.8);   
    }

     
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
    float distance = shine;
    vec3 l = vec3(1,1,0);
    l = normalize(l);
    float cosTheta=clamp(dot(n,l),0,1);
    vec3 R=reflect(-l,n);
    vec3 E=normalize(EyeDirection_cameraspace);
    float cosAlpha=clamp(dot(E,R),0,1);
    MaterialDiffuseColor += MaterialDiffuseColor *0.1;
    MaterialDiffuseColor=(MaterialDiffuseColor*.31) + (MaterialDiffuseColor*LightColor*LightPower*cosTheta/(distance*distance));

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
    vec3 finalColor = mix(vec3(0.8), MaterialDiffuseColor, fogFactor);
    color.rgb = finalColor;
    //color.rgb = MaterialDiffuseColor;
    
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //vec3 fc = color.rgb*vec3(grid(vec3(Position_worldspace.x,position0.y,Position_worldspace.z),.015));

//    vec3 fc = color.rgb*vec3(1.-grid2(Position_worldspace.xz/100.));
  //  color.rgb = fc;

   // color.rgb = mix(fc, color.rgb, smoothstep(0.36,.9,(1-depth*depth))).rgb;
    //color.rgb = vec3(grid(vec3(Position_worldspace.x,position0.y,Position_worldspace.z),.1));
    //color.r = gridf(Position_worldspace.x,0.1);
    //color.g = gridf(position0.y-35, 0.1);
    //color.b = gridf(Position_worldspace.z,0.1);
    //color.rgb = vec3(grid2(Position_worldspace.xz/100.));


    color.a=1;
    dcolor = vec4(vec3(depth), 1.0);
 }