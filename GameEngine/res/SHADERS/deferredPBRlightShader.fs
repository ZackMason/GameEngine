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

uniform vec3 u_viewpos;

// material parameters
float metallic;
float roughness;
float ao = .2;


const float kPi=3.14159265;
const float kShine = 16.0;
const float PI = 3.14159265359;
  
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

out vec4 FragColor;

void main()
{		
	vec3 FragPos = texture(u_pos, uv).rgb;
    vec3 Normal = texture(u_norm, uv).rgb;
    vec3 Albedo = texture(u_color, uv).rgb;
    float Specular = texture(u_color, uv).a;

	metallic =  clamp(FragPos.x/300., 0.01,1.0);
	roughness = clamp(FragPos.z/300. ,0.01,1.);

    vec3 N = normalize(Normal);
    vec3 V = normalize(u_viewpos - FragPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, Albedo, metallic);
	vec3 lr = vec3(0);
    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < num_lights; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(pointLights[i].position.xyz - FragPos.rgb);
        vec3 H = normalize(V + L);
        //float distance    = length(pointLights[i].position.xyz - FragPos.rgb);
        //float attenuation = 1.0 / (distance * distance);
		float distance=length(pointLights[i].position.xyz-FragPos);
		float attenuation=1./(pointLights[i].constant+pointLights[i].linear*distance+pointLights[i].quadratic*(distance*distance));
        vec3 radiance     = pointLights[i].color.rgb * attenuation;        
        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = mix( fresnelSchlick(max(dot(L,V),0.),F0) , fresnelSchlick(max(dot(H,V),0.),F0), step(.5,lp));
//        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
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
   
    FragColor = vec4(color,1.);
    //FragColor = vec4(lr,1.);
    //FragColor = vec4(vec3(metallic), 1.0);
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