#version 460
out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec4 position;
	vec4 color;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 50

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 vd;

uniform Light pointLights[NR_POINT_LIGHTS];
uniform Material material;

const float kPi=3.14159265;



// function prototypes
vec3 CalcDirLight(Light light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(Light light,vec3 normal,vec3 fragPos,vec3 viewDir);
//vec3 CalcSpotLight(Light light,vec3 normal,vec3 fragPos,vec3 viewDir);

bool blinn = true;
void main()
{
	// properties
	vec3 norm=normalize(Normal);

	
	// == =====================================================
	// Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
	// For each phase, a calculate function is defined that calculates the corresponding color
	// per lamp. In the main() function we take all the calculated colors and sum them up for
	// this fragment's final color.
	// == =====================================================
	// phase 1: directional lighting
	//vec3 result=CalcDirLight(dirLight,norm,viewDir);
	// phase 2: point lights
	vec3 result = vec3(0);
	//result = texture(material.diffuse,TexCoords).rgb;
	for(int i=0;i<NR_POINT_LIGHTS;i++)
	{
		if (pointLights[i].position.w >= 1.0)
			result+=CalcDirLight(pointLights[i],norm,vd);
		else
			result+=CalcPointLight(pointLights[i],norm,FragPos,vd);
	}
	// phase 3: spot light
	//result+=CalcSpotLight(spotLight,norm,FragPos,viewDir);
	FragColor=vec4(result,1.);
}

vec3 sdiffuse =texture(material.diffuse,TexCoords).rgb;
vec3 sspec = texture(material.specular,TexCoords).rgb;

// calculates the color when using a directional light.
vec3 CalcDirLight(Light light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir=normalize(-light.position.xyz);
	// diffuse shading
	float diff=max(dot(normal,lightDir),0.);
	// specular shading
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.),material.shininess);
	#if 1
	if(blinn)
	{
		const float kEnergyConservation=(8.+material.shininess)/(8.*kPi);
		vec3 halfwayDir=normalize(lightDir+viewDir);
		spec=kEnergyConservation*pow(max(dot(normal,halfwayDir),0.),material.shininess);
	}
	else
	{
		const float kEnergyConservation=(2.+material.shininess)/(2.*kPi);
		vec3 reflectDir=reflect(-lightDir,normal);
		spec=kEnergyConservation*pow(max(dot(viewDir,reflectDir),0.),material.shininess);
	}
	#endif
	// combine results
	vec3 ambient=light.ambient*sdiffuse;
	vec3 diffuse=light.diffuse*diff*sdiffuse*light.color.rgb;
	vec3 specular=light.specular*spec*sspec*light.color.rgb;
	//vec3 specular = vec3(0.);
	return(ambient+diffuse+specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(Light light,vec3 normal,vec3 fragPos,vec3 viewDir)
{
	vec3 lightDir=normalize(light.position.xyz-fragPos);
	// diffuse shading
	float diff=max(dot(normal,lightDir),0.);
	// specular shading
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.),material.shininess);
	#if 1
	if(blinn)
	{
		const float kEnergyConservation=(8.+material.shininess)/(8.*kPi);
		vec3 halfwayDir=normalize(lightDir+viewDir);
		spec=kEnergyConservation*pow(max(dot(normal,halfwayDir),0.),material.shininess);
	}
	else
	{
		const float kEnergyConservation=(2.+material.shininess)/(2.*kPi);
		vec3 reflectDir=reflect(-lightDir,normal);
		spec=kEnergyConservation*pow(max(dot(viewDir,reflectDir),0.),material.shininess);
	}
	#endif
	// attenuation
	float distance=length(light.position.xyz-fragPos);
	float attenuation=1./(light.constant+light.linear*distance+light.quadratic*(distance*distance));
	// combine results
	vec3 ambient=light.ambient*sdiffuse*light.color.rgb;
	vec3 diffuse=light.diffuse*diff*sdiffuse*light.color.rgb;
	vec3 specular=light.specular*spec*sspec*light.color.rgb;
	//vec3 specular=vec3(0.);

	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;

	return(ambient+diffuse+specular);
}

#if 0
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