#version 460
out vec4 FragColor;

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

uniform sampler2D u_pos;
uniform sampler2D u_color;
uniform sampler2D u_norm;

uniform vec3 u_viewpos;

const float kPi=3.14159265;
const float kShine = 16.0;

// function prototypes
//vec3 CalcDirLight(Light light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(Light light,vec3 normal,vec3 fragPos,vec3 viewDir, vec3 color, float s);
//vec3 CalcSpotLight(Light light,vec3 normal,vec3 fragPos,vec3 viewDir);

bool blinn = true;
void main()
{
	// deferred properties
	vec3 FragPos = texture(u_pos, uv).rgb;
    vec3 Normal = texture(u_norm, uv).rgb;
    vec3 Albedo = texture(u_color, uv).rgb;
    float Specular = texture(u_color, uv).a;
    
    // then calculate lighting as usual
    vec3 lighting = Albedo * 0.61; // hard-coded ambient component
	//vec3 lighting = vec3(0.);
    vec3 viewDir = (u_viewpos-FragPos);
	viewDir = normalize(viewDir);
    for(int i = 0; i < num_lights; ++i)
    {
        // diffuse
        //vec3 lightDir = normalize(pointLights[i].position.xyz - FragPos);
        //vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * pointLights[i].color.rgb;
        //lighting += diffuse;
		lighting += CalcPointLight(pointLights[i], Normal, FragPos, viewDir, Albedo, Specular);
	}
	FragColor = vec4(lighting,1.);
	//FragColor = vec4(vec3(1)*FragPos,1.);
	//FragColor = vec4(vec3(1)*Specular,1.);
	//FragColor = vec4(vec3(1)*viewDir,1.);
	//FragColor = vec4(vec3(1)*Albedo,1.);
	//FragColor = vec4(vec3(1)*Normal,1.);
	//FragColor = vec4(vec3(1)*Albedo,1.);

	
	#if 0 
	vec3 norm=normalize(u_norm);
	// == =====================================================
	// Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
	// For each phase, a calculate function is defined that calculates the corresponding color
	// per lamp. In the main() function we take all the calculated colors and sum them up for
	// this fragment's final color.
	// == =====================================================
	// phase 1: directional lighting
	//vec3 result=CalcDirLight(dirLight,norm,viewDir);
	// phase 2: point lights
	vec3 result = u_color.rgb;
	//result = texture(material.diffuse,TexCoords).rgb;
	for(int i=0;i<num_lights;i++)
	{
		if (pointLights[i].position.w >= 1.0)
			result+=CalcDirLight(pointLights[i],norm,vd);
		else
			result+=CalcPointLight(pointLights[i],norm,u_pos,vd);
	}
	// phase 3: spot light
	//result+=CalcSpotLight(spotLight,norm,FragPos,viewDir);
	FragColor=vec4(result,1.);
	#endif
}
// calculates the color when using a point light.
vec3 CalcPointLight(Light light,vec3 normal,vec3 fragPos,vec3 viewDir, vec3 color, float s)
{
	vec3 lightDir=normalize(light.position.xyz-fragPos);
	// diffuse shading
	float diff=max(dot(normal,lightDir),0.);
	// specular shading
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.),s);
	#if 1
	if(blinn)
	{
		const float kEnergyConservation=(8.+kShine)/(8.*kPi);
		vec3 halfwayDir=normalize(lightDir+viewDir);
		spec=kEnergyConservation*pow(max(dot(normal,halfwayDir),0.),kShine);
	}
	else
	{
		const float kEnergyConservation=(2.+kShine)/(2.*kPi);
		vec3 reflectDir=reflect(-lightDir,normal);
		spec=kEnergyConservation*pow(max(dot(viewDir,reflectDir),0.),kShine);
	}
	#endif
	// attenuation
	float distance=length(light.position.xyz-fragPos);
	float attenuation=1./(light.constant+light.linear*distance+light.quadratic*(distance*distance));
	// combine results
	vec3 ambient  =  light.ambient          * color * light.color.rgb;
	vec3 diffuse  =  light.diffuse  * diff  * color * light.color.rgb;
	vec3 specular =  light.specular * spec    * light.color.rgb;
	//vec3 specular=vec3(0.);

	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;

	return(ambient+diffuse+specular);
}



#if 0

// calculates the color when using a directional light.
vec3 CalcDirLight(Light light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir=normalize(-light.position.xyz);
	// diffuse shading
	float diff=max(dot(normal,lightDir),0.);
	// specular shading
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.),material.specular);
	#if 1
	if(blinn)
	{
		const float kEnergyConservation=(8.+material.specular)/(8.*kPi);
		vec3 halfwayDir=normalize(lightDir+viewDir);
		spec=kEnergyConservation*pow(max(dot(normal,halfwayDir),0.),material.specular);
	}
	else
	{
		const float kEnergyConservation=(2.+material.specular)/(2.*kPi);
		vec3 reflectDir=reflect(-lightDir,normal);
		spec=kEnergyConservation*pow(max(dot(viewDir,reflectDir),0.),material.specular);
	}
	#endif
	// combine results
	vec3 ambient=light.ambient*material.diffuse;
	vec3 diffuse=light.diffuse*diff*material.diffuse*light.color.rgb;
	vec3 specular=light.specular*spec*light.color.rgb;
	//vec3 specular = vec3(0.);
	return(ambient+diffuse+specular);
}


// calculates the color when using a spot light.
vec3 CalcSpotLight(Light light,vec3 normal,vec3 fragPos,vec3 viewDir)
{
	vec3 lightDir=normalize(light.position-fragPos);
	// diffuse shading
	float diff=max(dot(normal,lightDir),0.);
	// specular shading
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.),material.shininess);
	// attenuation
	float distance=length(light.position-fragPos);
	float attenuation=1./(light.constant+light.linear*distance+light.quadratic*(distance*distance));
	// spotlight intensity
	float theta=dot(lightDir,normalize(-light.direction));
	float epsilon=light.cutOff-light.outerCutOff;
	float intensity=clamp((theta-light.outerCutOff)/epsilon,0.,1.);
	// combine results
	vec3 ambient=light.ambient*vec3(texture(material.diffuse,TexCoords));
	vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,TexCoords));
	vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoords));
	ambient*=attenuation*intensity;
	diffuse*=attenuation*intensity;
	specular*=attenuation*intensity;
	return(ambient+diffuse+specular);
}
#endif