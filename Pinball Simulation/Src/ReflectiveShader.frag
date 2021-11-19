#version 460
out vec4 fragmentColour;

in VertexData {
vec3 FragPos;
vec3 Normal;
vec2 texCoord;
} fs_in;	

uniform vec3 cameraPos;
uniform samplerCube skyBox;

vec3 newNormal;
void main() {
	vec3 I  = normalize(fs_in.FragPos -cameraPos);
	vec3 R = reflect(I, normalize(fs_in.Normal));
	fragmentColour = vec4(texture(skyBox, R).rgb, 1.0);
};