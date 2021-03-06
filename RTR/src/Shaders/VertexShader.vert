#version 460
layout (location = 0) in vec3 vertexPoints;
layout (location = 1) in vec3 vertexCol;
out vec3 vertexColour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform mat4 transformMat;
void main()
{
	vertexColour = vertexCol;
	gl_Position = projection * view * model *  vec4(vertexPoints.x,vertexPoints.y,vertexPoints.z, 1.0) ;
	//gl_Position = vec4(vertexPoints.x,vertexPoints.y,vertexPoints.z, 1.0);

};