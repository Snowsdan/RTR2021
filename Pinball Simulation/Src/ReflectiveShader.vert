#version 460
layout (location = 0) in vec3 vertexPoints;

out VertexData {
    vec3 FragPos;
    vec2 texCoord;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    vs_out.FragPos =  vec3(model * vec4(vertexPoints, 1.0));
	gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);

};