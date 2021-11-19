#version 460
layout (location = 0) in vec3 vertexPoints;
layout (location = 1) in vec3 inNormal;

out vec3 Normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

void main()
{
	//Lerp between coordinates for a sphere and the original cube coordinates
	
	float r = 1.0;
	vec3 newPosition;
	float theta = acos(vertexPoints.x);
	float phi = asin(vertexPoints.y);
	newPosition.x = r * sin(phi) * cos(theta);
	newPosition.y = r * sin(phi) * sin(theta);
	newPosition.z =  r * cos(phi);

	vec3 finalPosition = (1 - time) * vertexPoints + time * newPosition;

	gl_Position = projection * view * model *  vec4(finalPosition, 1.0);
	fragPos = vec3(model * vec4(vertexPoints, 1.0));
	Normal = inNormal;

};