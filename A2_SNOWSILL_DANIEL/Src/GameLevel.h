#pragma once
#include <vector>
#include <iostream>
#include "RTRObject.h"
#include "RTRShader.h"
#include "RTRLighting.h"

class GameLevel {
public:

	GameLevel(glm::mat4 projectionMat);
	~GameLevel();

	//Object Rendering lists
	std::vector<RTRObject*> defaultShaderObjects;
	std::vector<RTRObject*> plasmaShaderObjects;

	//Object Physics lists
	std::vector<RTRObject*> dynamicObjects;			//Stores moving objects for physics update purposes
	std::vector<RTRObject*> staticObjects;			//Stores static objects for physics update purposes


	void RenderScene(float currentTime, float deltaTime, glm::mat4 viewMatrix, RTRCamera* camera);
	void AddCube(glm::vec3 startingPosition);
	void AddSphere(glm::vec3 startingPosition);
	void UpdateState();

private:
	RTRShader* defaultShader{ nullptr };
	RTRShader* plasmaShader{ nullptr };
	RTRLightingModel* lightingModel;
	glm::mat4 projectionMatrix{ 1.0f };

};