#version 460
layout (location = 0) in vec3 vertexPoints;
layout (location = 1) in vec3 inNormal;

out vec3 Normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model *  vec4(vertexPoints.x,vertexPoints.y,vertexPoints.z, 1.0);
	fragPos = vec3(model * vec4(vertexPoints, 1.0));
	Normal = inNormal;

};