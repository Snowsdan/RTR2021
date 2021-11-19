#version 460
#define MAX_POINT_LIGHTS 10

in vec3 fragPos;
in vec3 Normal;

out vec4 fragmentColour;


struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	int type; //0 = direction, 1 = point
	vec3 direction;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};


vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, Material currentMaterial);
vec3 CalcPointLight(Light light, vec3 normal, vec3 viewDir, Material currentMaterial);


uniform Light lights[MAX_POINT_LIGHTS];
uniform int numActiveLights;
uniform bool isLighting;
//uniform Material material;

uniform Material xMaterial;
uniform Material yMaterial;
uniform Material zMaterial;

uniform vec3 viewPos;

void main() {
	
	vec3 norm = normalize(Normal);
	vec3 viewDirection = normalize(viewPos - fragPos);
	vec3 finalColour = vec3(0.0);

	if(isLighting){
		Material currentMaterial;
		//Set the material for the fragment depending on the normal
		//Use < and > operators as comparing floats isn't reliable
		if(Normal.x > 0.5 || Normal.x < -0.5){
			currentMaterial = xMaterial;
		}	
		else if(Normal.y > 0.5 || Normal.y < -0.5){
			currentMaterial = yMaterial;
		}
		else if(Normal.z > 0.5 || Normal.z < -0.5){
			currentMaterial = zMaterial;
		}
		
		//Apply lighting from each active light source
		for(int i = 0; i < numActiveLights; i++){
			if(lights[i].type == 0)	//Directional Lighting
				finalColour +=  CalcDirLight(lights[0], norm, viewDirection, currentMaterial);
			else if(lights[i].type == 1)//Point lighting
				finalColour += CalcPointLight(lights[i], norm, viewDirection, currentMaterial);
		}
	} 
	else {

		if(Normal.x > 0.5 || Normal.x < -0.5)
			finalColour = xMaterial.ambient +  xMaterial.diffuse; 
		else if(Normal.y > 0.5 || Normal.y < -0.5)
			finalColour = yMaterial.ambient + yMaterial.diffuse; 
		else if(Normal.z > 0.5 || Normal.z < -0.5)
			finalColour = zMaterial.ambient + zMaterial.diffuse;
	}

	
	//finalColour += CalcPointLight(lights[1], norm, fragPos, viewDirection);
	//finalColour = CalcPointLight(lights[0], norm, fragPos, viewDirection);
	//finalColour = CalcDirLight(lights[0], norm, viewDirection);

	fragmentColour = vec4(finalColour,1.0);
	
};

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, Material currentMaterial){
	float ambientStrength = 0.1;
	vec3 lightDirection = normalize(-light.direction);

	float diff = max(dot(normal, lightDirection), 0.0);


	//TODO: figure out why reflection is going the wrong way

	//Specular
	//First vector needs to be pointing FROM light source. 
	//lightDirection is pointing towards it, so make it negative
	vec3 halfwayDir = normalize(lightDirection + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), currentMaterial.shininess);

	vec3 ambient = (ambientStrength * currentMaterial.ambient) * light.ambient;
	vec3 diffuse = diff * currentMaterial.diffuse * light.diffuse;
	vec3 specular = currentMaterial.specular * spec * light.specular;

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 viewDir,  Material currentMaterial){
	//Ambient
	float ambientStrength = 0.1;
	vec3 ambient = (ambientStrength * currentMaterial.ambient) * light.ambient ;

	float dist= length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist +
		light.quadratic * (dist * dist));

	//Diffuse
	vec3 lightDirection = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = (diff * currentMaterial.diffuse) * light.diffuse;

	
	//Specular
	//First vector needs to be pointing FROM light source. 
	//lightDirection is pointing towards it, so make it negative
	vec3 halfwayDir = normalize(lightDirection + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), currentMaterial.shininess);
	vec3 specular = currentMaterial.specular * spec * light.specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

