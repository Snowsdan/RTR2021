#version 460
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 TexCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform float u_CurTime;
uniform float u_TimeDelta;

out VertexData{
	vec3 FragPos;
	vec2 texCoord;
} vs_out;

vec3 lerp(vec3 v0, vec3 v1, float t){
return (1.0 - t) * v0 + t * v1;
}

void main(){
	vs_out.texCoord = TexCoord;
	vs_out.FragPos = vec3(u_ModelMatrix * vec4(a_Position, 1.0));
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(vs_out.FragPos, 1.0);
}