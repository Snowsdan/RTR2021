#pragma once

#include "RTRShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum LightType {
	directional, point
};

class Light {
	public:
		Light(LightType type, glm::vec3 position, glm::vec3 dir, glm::vec3 ambientVec,
			glm::vec3 diffuseVec, glm::vec3 specularVec, float constant, float linear, float quadratic);
		void DrawLightModel();
		void DrawLight(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix);

		LightType lightType;

		glm::vec3 lightPos;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		glm::vec3 direction;
		glm::vec3 position;

		float constant;
		float linear;
		float quadratic;

	private:
		
		RTRShader* lightShader = new RTRShader("src/Shaders/LightVertexShader.vert", "src/Shaders/LightFragmentShader.frag");

};