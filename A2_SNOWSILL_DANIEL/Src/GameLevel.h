#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <tuple>
#include "RTRObject.h"
#include "RTRShader.h"
#include "RTRLighting.h"
#include "SkyBox.h"
#include "Pegs.h"
#

struct GridCell {
	std::vector<RTRObject*> cellObjects;
};

class GameLevel {
public:

	GameLevel(glm::mat4 projectionMat);
	~GameLevel();

	//Object Rendering lists
	std::vector<RTRObject*> defaultShaderObjects;
	std::vector<RTRObject*> plasmaShaderObjects;
	std::vector<RTRObject*> reflectiveShaderObjects;
	//Object Physics lists
	std::vector<RTRObject*> dynamicObjects;			//Stores moving objects for physics update purposes
	std::vector<RTRObject*> staticObjects;			//Stores static objects for physics update purposes

	std::vector<RTRObject*> debugObjects;			//Holds any objects that are drawn for debug (e.g lights)

	
	void RenderScene(float currentTime, float deltaTime, glm::mat4 viewMatrix, RTRCamera* camera);
	RTRObject* AddCube(glm::vec3 startingPosition, glm::vec3 scale, const char* texturePath);
	RTRObject* AddSphere(glm::vec3 startingPosition);
	void UpdateState(float dt);
	void LaunchBall();
	bool debug = false;

private:
	RTRCube* gameBoardBase;						//The visual for the board base that collisions will not be tested on
	void InitGameBoard();
	void CreateUniformGrid();
	void MoveFlippers();
	void HandleCollision(RTRSphere* obj, CollisionInfo* collision);

	//Debug helper methods
	void DrawDebug(glm::mat4 view, glm::mat4 projection);
	void DrawBoundingVolumes();
	void DrawUniformGrid();
	void AddDebugLight(RTRLight_t* light);

	std::map<std::tuple<int,int>, std::vector<RTRObject*>*> uniformGrid;	//Use a map as grid cells can be instantly accessed through the tuple

	RTRObject* leftFlipper;
	RTRObject* rightFlipper;
	RTRShader* defaultShader{ nullptr };
	RTRShader* plasmaShader{ nullptr };
	RTRShader* skyBoxShader{ nullptr };
	RTRShader* basicShader{ nullptr };				//A basic shader used to just render geometry in one colour
	RTRShader* reflectiveShader{ nullptr };

	SkyBox* skyBox{ nullptr };
	RTRLightingModel* lightingModel;
	glm::mat4 projectionMatrix{ 1.0f };

	int uniformRows{ 10 };
	int uniformColumns{ 10 };
	float cellSize{ 1.0 };
	glm::vec3 startingPosition = glm::vec3(-5.0, 0.0, -5.0);
};