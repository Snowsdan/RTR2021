#version 460
out vec4 fragmentColour;

uniform vec3 colour;
void main() {
	fragmentColour = vec4(colour, 1.0);
};