#version 460
layout (location = 0) in vec3 vertexPoints;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model *  vec4(vertexPoints.x,vertexPoints.y,vertexPoints.z, 1.0) ;
};