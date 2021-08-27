#version 460
layout (location = 0) in vec3 vertexPoints;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in mat4 instanceMatrix;

out vec3 Normal;
out vec3 fragPos;

uniform mat4 view;
uniform mat4 projection;

//uniform mat4 modelMatrices[9];

void main()
{
	//vec3 transform = positionTranform[gl_InstanceID];

	gl_Position = projection * view * instanceMatrix *  vec4(vertexPoints, 1.0);
	fragPos = vec3(instanceMatrix * vec4(vertexPoints, 1.0));
	//Normal = mat3(transpose(inverse(model))) * inNormal;
	Normal = inNormal;

};