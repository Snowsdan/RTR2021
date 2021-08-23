#version 460
in vec3 vertexColour;
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


vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

#define MAX_POINT_LIGHTS 4
uniform Light lights[MAX_POINT_LIGHTS];


//uniform Light light;
//uniform DirLight dirLight;
uniform Material material;

uniform vec3 viewPos;
//uniform bool isLighting;

void main() {
	

	vec3 norm = normalize(Normal);
	vec3 viewDirection = normalize(viewPos - fragPos);
	vec3 finalColour = vec3(1.0);
	//Directional lighting
	

	//Point lighting
//	for(int i = 0; i < lights.length(); i++){
//		if(lights[0].type == 0){
//		finalColour += CalcDirLight(lights[i], norm, lights[i].direction);
//		}
//		else{
//		finalColour += CalcPointLight(lights[i], norm, fragPos, lights[i].position);
//		}
//		
//	}
	finalColour = CalcPointLight(lights[0], norm, fragPos, lights[0].position);
	fragmentColour = vec4(finalColour,1.0);
	
};

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir){

	vec3 ambient = material.ambient * light.ambient;

	vec3 lightDirection = normalize(-light.direction);

	//Diffuse
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = (diff * material.diffuse) * light.diffuse;

	//Specular
	//First vector needs to be pointing FROM light source. 
	//lightDirection is pointing towards it, so make it negative
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess);
	vec3 specular = (material.specular * spec) * light.specular;


	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir){
	//Ambient
	vec3 ambient = material.ambient * light.ambient ;

	float dist= length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist +
		light.quadratic * (dist * dist));

	//TODO: set different colours for each axis
	//Diffuse
	vec3 lightDirection = normalize(light.position - fragPos);

	//vec3 lightDirection = normalize(-light.direction);
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = (diff * material.diffuse) * light.diffuse;

	//Specular
	//First vector needs to be pointing FROM light source. 
	//lightDirection is pointing towards it, so make it negative
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess);
	vec3 specular = (material.specular * spec) * light.specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

