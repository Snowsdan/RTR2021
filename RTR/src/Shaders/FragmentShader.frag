#version 400
in vec3 vertexColour;
out vec4 fragmentColour;
//"uniform vec4 ourColour;"
void main() {
	fragmentColour = vec4(vertexColour,1.0f);
};