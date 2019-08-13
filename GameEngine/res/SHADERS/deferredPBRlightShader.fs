#version 460

struct Light{
	vec4 position;
	vec4 color;
	
	float constant;
	float linear;
	float quadratic;
	
	float ambient;
	float diffuse;
	float specular;
};


#define NR_LIGHTS 100

in vec2 uv;

uniform Light pointLights[NR_LIGHTS];
uniform int num_lights;

uniform float lp;

uniform sampler2D u_pos;
uniform sampler2D u_color;
uniform sampler2D u_norm;
uniform sampler2D u_last_frame;

uniform vec3 u_viewpos;

uniform mat4 u_projection;
uniform mat4 u_inv_projection;
uniform mat4 u_view;
uniform mat4 u_inv_view;

// material parameters
float metallic;
float roughness;
float ao = .2;


const float kPi=3.14159265;
const float kShine = 16.0;
const float PI = 3.14159265359;
const float rayStep=.021;
const float minRayStep=.1;
const int maxSteps=30;
const float searchDist=5.;
const int numBinarySearchSteps=10;
const float reflectionSpecularFalloffExponent=24.;
  
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec4 RayCast(in vec3 dir, inout vec3 hitCoord, out float dDepth);
vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth);
vec3 hash(vec3 n);
vec3 PositionFromDepth(float depth);
float fresnel(vec3 direction,vec3 normal);


 out vec4 FragColor;

void main()
{		
	vec3 FragPos = texture(u_pos, uv).rgb;
    vec3 Normal = texture(u_norm, uv).rgb;
    vec3 Albedo = texture(u_color, uv).rgb;
    float Specular = texture(u_color, uv).a;
    vec3 last = texture(u_last_frame,uv).rgb;
    metallic = texture(u_pos,uv).a;
    roughness = texture(u_norm,uv).a;
    vec3 N = normalize(Normal);
    vec3 V = normalize(u_viewpos - FragPos);



    //IOR
    // plastic
    vec3 F0 = vec3(0.04); 

    // gold
    //vec3 F0 = vec3(1.,.71,.29); 
    F0 = mix(F0, Albedo, metallic);
	vec3 lr = vec3(0);
    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < num_lights; ++i) 
    {
        // calculate per-light radiance
        vec3 L, radiance;
        if (pointLights[i].position.w >=1.0)
        {
            L = normalize(pointLights[i].position.xyz);
        }
        else
        {
            L = normalize(pointLights[i].position.xyz - FragPos.rgb);
        }

        vec3 H = normalize(V + L);
        //float distance    = length(pointLights[i].position.xyz - FragPos.rgb);
        //float attenuation = 1.0 / (distance * distance);
		float distance=length(pointLights[i].position.xyz-FragPos);
		float attenuation=1./(pointLights[i].constant+pointLights[i].linear*distance+pointLights[i].quadratic*(distance*distance));
        if (pointLights[i].position.w >=.90)
            radiance = pointLights[i].color.rgb;
        else
            radiance     = pointLights[i].color.rgb * attenuation;        
        // cook-torrance brdf

        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        //vec3 F    = mix( fresnelSchlick(max(dot(L,V),0.),F0) , fresnelSchlick(max(dot(H,V),0.),F0), step(.5,lp));
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular     = numerator / max(denominator, 0.001);  
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * Albedo / PI + specular) * radiance * NdotL; 
    }   
  
    vec3 ambient = vec3(0.03) * Albedo * ao;
    vec3 color = ambient + Lo;
	
    //color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0/2.2));  
    //FragColor = vec4(Albedo,1.);
   // FragColor = vec4(vec3(roughness), 1.0);

    //if (uv.x>lp)
    if (roughness<5. || true)
    {
        float dDepth;
        vec3 VN = (u_view * vec4(Normal,.0)).xyz;
        vec3 VV = (u_view * vec4(V,1.0)).xyz;
        vec3 hitPos = ( u_view * vec4(FragPos,1.0)).xyz;
        vec3 reflected = normalize(reflect(normalize(hitPos),normalize(VN)));
        vec3 hh = hitPos;
        vec3 jitt = mix(vec3(0.),vec3(hash(FragPos)),roughness)*0.;

        vec4 coords = RayCast(jitt + reflected * max(minRayStep,-hitPos.z),hh,dDepth);
        vec2 dCoords = smoothstep(0.2,.6,abs(vec2(0.5)-coords.xy));
        float screenEdgeFactor = clamp(1. - (dCoords.x + dCoords.y),0.,1.);

        float multiplier = pow(1.-roughness,reflectionSpecularFalloffExponent) * 
                        screenEdgeFactor *
                        -reflected.z;

        vec3 ssr = texture(u_last_frame, coords.xy).rgb * clamp(multiplier,0.,.9);

        float fres = fresnel(V,N);
        FragColor = vec4(mix(fres*ssr+color,color,lp),1.);
    }
    else
        FragColor = vec4(color,1.);
    vec4 puv = (u_projection * u_view * vec4(FragPos,1.));
    //FragColor.rg = (puv.xy/puv.w) ;//* .5 + .5;
    //FragColor.rgb = Normal;
    //FragColor.rgb = vec3(multiplier);
    //FragColor.b =0;
}  

#define fresnelExp 5.

float fresnel(vec3 direction,vec3 normal){
    vec3 halfDirection=normalize(normal+direction);
    
    float cosine=dot(halfDirection,direction);
    float product=max(cosine,0.);
    float factor=1.-pow(product,fresnelExp);
    
    return factor;
}


vec3 hash(vec3 n)
{
    n = fract(n * vec3(.8));
    n += dot(n,n.yxz+19.19);
    return fract((n.xxy+n.yxx)*n.zyx);
}

vec3 PositionFromDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;

    vec4 clipSpacePosition = vec4(uv * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePosition = u_inv_projection * clipSpacePosition;

    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;

    return viewSpacePosition.xyz;
}


vec4 RayCast(in vec3 dir,inout vec3 hitCoord,out float dDepth)
{
    dir *= rayStep;
    float depth = 0.;
    int steps = 0;
    vec4 projectedCoord = vec4(0.);

    for (int i = 0; i < maxSteps; ++i)
    {
        hitCoord += dir;

        projectedCoord = u_projection * vec4(hitCoord,1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * .5 + .5;
        depth = (u_view * vec4(textureLod(u_pos, projectedCoord.xy,0.0).xyz,1.0)).z;

        //if (depth>1000.)
          //  continue;
        dDepth = hitCoord.z - depth;
        if ((dir.z - dDepth) < 0.2)
        {
            if(dDepth <= 0.0)
            {
                vec4 Result;
                Result = vec4(BinarySearch(dir,hitCoord,dDepth),1.0);
                return Result;
            }
        }
        steps++;
    }
    return vec4(projectedCoord.xy,depth,1.);
}

vec3 BinarySearch(inout vec3 dir,inout vec3 hitCoord,inout float dDepth)
{
    float depth;
    vec4 projectedCoord;

    for (int i = 0; i < numBinarySearchSteps; ++i)
    {
        projectedCoord = u_projection * vec4(hitCoord,1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * .5 + .5;
        depth = (u_view * vec4(textureLod(u_pos, projectedCoord.xy,0.0).xyz,1.0)).z;

//        depth = textureLod(u_pos, projectedCoord.xy,2.0).z;

        dDepth = hitCoord.z - depth;

        dir *= .5;
        if (dDepth > 0.) 
            hitCoord += dir;
        else
            hitCoord -= dir;
    }

    projectedCoord = u_projection * vec4(hitCoord, 1.0);
    projectedCoord.xy /= projectedCoord.w;
    projectedCoord.xy = projectedCoord.xy * .5 + .5;

    return vec3(projectedCoord.xy,depth);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta,vec3 F0)
{
	return F0+(1.-F0)*pow(1.-cosTheta,5.);
}